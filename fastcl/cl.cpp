#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define CL_USE_DEPRECATED_OPENCL_2_1_APIS
#define CL_USE_DEPRECATED_OPENCL_2_2_APIS

#include <iostream>
#include "cl.h"

struct api_struct
{
    boost::dll::shared_library lib;

    api_struct() : lib("OpenCL", boost::dll::load_mode::search_system_folders | boost::dll::load_mode::append_decorations)
    {

    }
};

struct _cl_kernel
{
    void* ptr = nullptr;
    std::atomic_uint refc{0};
};

static api_struct api;

template<typename T>
auto& get_ptr(const char* name)
{
    return api.lib.get<T>(name);
}

#define IMPORT(name) auto name##_ptr = get_ptr<decltype(name)>(#name);

template<typename R, typename... Args>
auto detect_args(R(*func)(Args...)){return std::tuple<Args...>();}

template<typename R, typename... Args>
auto detect_return(R(*func)(Args...)){return R();}

template<typename T>
auto to_native_type(T&& in)
{
    return std::forward<T>(in);
}

template<typename T>
auto from_native_type(T&& in)
{
    return in;
}

_cl_kernel* to_native_type(_cl_kernel* in)
{
    assert(in->ptr);

    return reinterpret_cast<_cl_kernel*>(in->ptr);
}

_cl_kernel* from_native_type(_cl_kernel* in)
{
    _cl_kernel* ptr = new _cl_kernel();
    ptr->ptr = in;
    ptr->refc = 1;
    return ptr;
}

template<typename Func, typename... T>
auto call(Func f, T&&... args)
{
    if constexpr(std::is_same_v<decltype(detect_return(f)), void>)
        return f(to_native_type(args)...);
    else
        return from_native_type(f(to_native_type(args)...));
}

IMPORT(clRetainKernel);
IMPORT(clReleaseKernel);

cl_int clRetainKernel(cl_kernel kern)
{
    if(kern == nullptr)
        return CL_INVALID_KERNEL;

    kern->refc++;
    return call(clRetainKernel_ptr, kern);
}

cl_int clReleaseKernel(cl_kernel kern)
{
    if(kern == nullptr)
        return CL_INVALID_KERNEL;

    bool should_delete = (--kern->refc) == 0;
    cl_int val = call(clReleaseKernel_ptr, kern);

    if(should_delete)
        delete kern;

    return val;
}

IMPORT(clCreateKernelsInProgram);

cl_int clCreateKernelsInProgram(cl_program program, cl_uint num_kernels, cl_kernel* kernels, cl_uint* num_kernels_ret)
{
    cl_int err = clCreateKernelsInProgram_ptr(program, num_kernels, kernels, num_kernels_ret);

    if(num_kernels != 0 && kernels)
    {
        for(cl_uint i=0; i < num_kernels; i++)
        {
            kernels[i] = from_native_type(kernels[i]);
        }
    }

    return err;
}

#define NAME_TYPE(name, idx) std::remove_cvref_t<decltype(std::get<idx>(detect_args(name##_ptr)))>
#define NAME_RETURN(name) std::remove_cvref_t<decltype(detect_return(name##_ptr))>

#define SHIM_0(name) IMPORT(name); auto name(void) -> NAME_RETURN(name) {return call(name##_ptr);}
#define SHIM_1(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0) -> NAME_RETURN(name) {return call(name##_ptr, a0);}
#define SHIM_2(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1);}
#define SHIM_3(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2);}
#define SHIM_4(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3);}
#define SHIM_5(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4);}
#define SHIM_6(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5);}
#define SHIM_7(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6);}
#define SHIM_8(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7);}
#define SHIM_9(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8);}
#define SHIM_10(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);}
#define SHIM_11(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);}
#define SHIM_12(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);}
#define SHIM_13(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11, NAME_TYPE(name, 12) a12) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);}
#define SHIM_14(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11, NAME_TYPE(name, 12) a12, NAME_TYPE(name, 13) a13) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);}
#define SHIM_15(name) IMPORT(name); auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11, NAME_TYPE(name, 12) a12, NAME_TYPE(name, 13) a13, NAME_TYPE(name, 14) a14) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);}

//SHIM_1(clRetainKernel);
//SHIM_1(clReleaseKernel);

SHIM_3(clGetPlatformIDs);
SHIM_5(clGetPlatformInfo);
SHIM_5(clGetDeviceIDs);
SHIM_5(clGetDeviceInfo);
SHIM_5(clCreateSubDevices);
SHIM_1(clRetainDevice);
SHIM_1(clReleaseDevice);
SHIM_3(clSetDefaultDeviceCommandQueue);
SHIM_3(clGetDeviceAndHostTimer);
SHIM_2(clGetHostTimer);
SHIM_6(clCreateContext);
SHIM_5(clCreateContextFromType);
SHIM_1(clRetainContext);
SHIM_1(clReleaseContext);
SHIM_5(clGetContextInfo);
SHIM_3(clSetContextDestructorCallback);
SHIM_4(clCreateCommandQueueWithProperties);
SHIM_1(clRetainCommandQueue);
SHIM_1(clReleaseCommandQueue);
SHIM_5(clGetCommandQueueInfo);
SHIM_5(clCreateBuffer);
SHIM_5(clCreateSubBuffer);
SHIM_6(clCreateImage);
SHIM_6(clCreatePipe);
SHIM_6(clCreateBufferWithProperties);
SHIM_7(clCreateImageWithProperties);
SHIM_1(clRetainMemObject);
SHIM_1(clReleaseMemObject);
SHIM_6(clGetSupportedImageFormats);
SHIM_5(clGetMemObjectInfo);
SHIM_5(clGetImageInfo);
SHIM_5(clGetPipeInfo);
SHIM_3(clSetMemObjectDestructorCallback);
SHIM_4(clSVMAlloc);
SHIM_2(clSVMFree);
SHIM_3(clCreateSamplerWithProperties);
SHIM_1(clRetainSampler);
SHIM_1(clReleaseSampler);
SHIM_5(clCreateProgramWithSource);
SHIM_7(clCreateProgramWithBinary);
SHIM_5(clCreateProgramWithBuiltInKernels);
SHIM_4(clCreateProgramWithIL);
SHIM_1(clRetainProgram);
SHIM_1(clReleaseProgram);
SHIM_6(clBuildProgram);
SHIM_9(clCompileProgram);
SHIM_9(clLinkProgram);
SHIM_3(clSetProgramReleaseCallback);
SHIM_4(clSetProgramSpecializationConstant);
SHIM_1(clUnloadPlatformCompiler);
SHIM_5(clGetProgramInfo);
SHIM_6(clGetProgramBuildInfo);
SHIM_3(clCreateKernel);
//SHIM_4(clCreateKernelsInProgram);
SHIM_2(clCloneKernel);
//SHIM_1(clRetainKernel);
//SHIM_1(clReleaseKernel);
SHIM_4(clSetKernelArg);
SHIM_3(clSetKernelArgSVMPointer);
SHIM_4(clSetKernelExecInfo);
SHIM_5(clGetKernelInfo);
SHIM_6(clGetKernelArgInfo);
SHIM_6(clGetKernelWorkGroupInfo);
SHIM_8(clGetKernelSubGroupInfo);
SHIM_2(clWaitForEvents);
SHIM_5(clGetEventInfo);
SHIM_2(clCreateUserEvent);
SHIM_1(clRetainEvent);
SHIM_1(clReleaseEvent);
SHIM_2(clSetUserEventStatus);
SHIM_4(clSetEventCallback);
SHIM_5(clGetEventProfilingInfo);
SHIM_1(clFlush);
SHIM_1(clFinish);
SHIM_9(clEnqueueReadBuffer);
SHIM_14(clEnqueueReadBufferRect);
SHIM_9(clEnqueueWriteBuffer);
SHIM_14(clEnqueueWriteBufferRect);
SHIM_9(clEnqueueFillBuffer);
SHIM_9(clEnqueueCopyBuffer);
SHIM_13(clEnqueueCopyBufferRect);
SHIM_11(clEnqueueReadImage);
SHIM_11(clEnqueueWriteImage);
SHIM_8(clEnqueueFillImage);
SHIM_9(clEnqueueCopyImage);
SHIM_9(clEnqueueCopyImageToBuffer);
SHIM_9(clEnqueueCopyBufferToImage);
SHIM_10(clEnqueueMapBuffer);
SHIM_12(clEnqueueMapImage);
SHIM_6(clEnqueueUnmapMemObject);
SHIM_7(clEnqueueMigrateMemObjects);
SHIM_9(clEnqueueNDRangeKernel);
SHIM_10(clEnqueueNativeKernel);
SHIM_4(clEnqueueMarkerWithWaitList);
SHIM_4(clEnqueueBarrierWithWaitList);
SHIM_8(clEnqueueSVMFree);
SHIM_8(clEnqueueSVMMemcpy);
SHIM_8(clEnqueueSVMMemFill);
SHIM_8(clEnqueueSVMMap);
SHIM_5(clEnqueueSVMUnmap);
SHIM_8(clEnqueueSVMMigrateMem);
SHIM_8(clCreateImage2D);
SHIM_10(clCreateImage3D);
SHIM_2(clEnqueueMarker);
SHIM_3(clEnqueueWaitForEvents);
SHIM_1(clEnqueueBarrier);
SHIM_0(clUnloadCompiler);
//SHIM_1(clGetExtensionFunctionAddress);
SHIM_4(clCreateCommandQueue);
SHIM_5(clCreateSampler);
SHIM_5(clEnqueueTask);

