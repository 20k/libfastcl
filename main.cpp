#include <iostream>
#include "fastcl/cl.h"
#include <vector>
#include <string.h>
#include <assert.h>

static
void get_platform_ids(cl_platform_id* clSelectedPlatformID)
{
    char chBuffer[1024] = {};
    cl_uint num_platforms;
    std::vector<cl_platform_id> clPlatformIDs;
    cl_int ciErrNum;
    *clSelectedPlatformID = NULL;
    cl_uint i = 0;

    ciErrNum = clGetPlatformIDs(0, NULL, &num_platforms);

    if(ciErrNum != CL_SUCCESS)
    {
        throw std::runtime_error("Bad clGetPlatformIDs call " + std::to_string(ciErrNum));
    }
    else
    {
        if(num_platforms == 0)
        {
            throw std::runtime_error("No available platforms");
        }
        else
        {
            clPlatformIDs.resize(num_platforms);

            ciErrNum = clGetPlatformIDs(num_platforms, &clPlatformIDs[0], NULL);

            for(i = 0; i < num_platforms; i++)
            {
                ciErrNum = clGetPlatformInfo(clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &chBuffer, NULL);

                if(ciErrNum == CL_SUCCESS)
                {
                    if(strstr(chBuffer, "NVIDIA") != NULL || strstr(chBuffer, "AMD") != NULL)// || strstr(chBuffer, "Intel") != NULL)
                    {
                        *clSelectedPlatformID = clPlatformIDs[i];
                    }
                }
            }

            if(*clSelectedPlatformID == NULL)
            {
                *clSelectedPlatformID = clPlatformIDs[num_platforms-1];
            }
        }
    }
}

std::string kstr = R"(
__kernel
void test_name()
{
    printf("Hello");
}
)";

///TODO:
///detect unbuilt programs
///detect kernels with unset arguments
///improve performance

int main()
{
    cl_platform_id pid = {};
    get_platform_ids(&pid);

    cl_uint num_devices = 0;
    cl_device_id devices[100] = {};

    clGetDeviceIDs(pid, CL_DEVICE_TYPE_GPU, 1, devices, &num_devices);

    cl_device_id selected_device = devices[0];

    cl_context_properties props[] =
    {
        CL_CONTEXT_PLATFORM, (cl_context_properties)pid,
        0
    };

    cl_int error;
    cl_context ctx = clCreateContext(props, 1, &selected_device, nullptr, nullptr, &error);

    assert(error == CL_SUCCESS);

    const char* ptr = kstr.c_str();

    cl_program prog = clCreateProgramWithSource(ctx, 1, &ptr, nullptr, &error);

    assert(error == CL_SUCCESS);

    cl_int build_err = clBuildProgram(prog, 1, &selected_device, "", nullptr, nullptr);

    assert(prog);

    std::cout << "hi\n";

    cl_uint num = 0;
    cl_int err = clCreateKernelsInProgram(prog, 0, nullptr, &num);

    std::cout << "Hello\n";

    if(err != CL_SUCCESS)
    {
        std::cout << "Error creating program " << err << std::endl;
        throw std::runtime_error("Bad Program");
    }

    std::cout << "Num " << num << std::endl;

    std::vector<cl_kernel> cl_kernels;
    cl_kernels.resize(num + 1);

    clCreateKernelsInProgram(prog, num, cl_kernels.data(), nullptr);

    cl_kernels.resize(num);

    cl_command_queue cqueue = clCreateCommandQueue(ctx, selected_device, 0, nullptr);

    size_t global[1] = {1};
    size_t local[1] = {1};
    size_t offset[1] = {0};

    clEnqueueNDRangeKernel(cqueue, cl_kernels.at(0), 1, offset, global, local, 0, nullptr, nullptr);

    clFinish(cqueue);

    printf("Done!\n");

    return 0;
}
