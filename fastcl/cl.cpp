#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define CL_USE_DEPRECATED_OPENCL_2_1_APIS
#define CL_USE_DEPRECATED_OPENCL_2_2_APIS

#include <nlohmann/json.hpp>
#include <iostream>
#include "cl.h"

template<typename T>
auto to_native_type(T&& in)
{
    return std::forward<T>(in);
}

_cl_kernel* to_native_type(_cl_kernel* in);
_cl_program* to_native_type(_cl_program* in);

struct api_struct
{
    boost::dll::shared_library lib;

    api_struct() : lib("OpenCL", boost::dll::load_mode::search_system_folders | boost::dll::load_mode::append_decorations)
    {

    }
};

static api_struct api;

template<typename T>
auto& get_ptr(const char* name)
{
    return api.lib.get<T>(name);
}


struct ref_counting
{
    std::atomic_uint refc{0};

    void inc()
    {
        refc++;
    }

    bool dec()
    {
        return (--refc) == 0;
    }
};

#define IMPORT(name) inline auto name##_ptr = get_ptr<decltype(name)>(#name);

IMPORT(clGetPlatformIDs);
IMPORT(clGetPlatformInfo);
IMPORT(clGetDeviceIDs);
IMPORT(clGetDeviceInfo);
IMPORT(clCreateSubDevices);
IMPORT(clRetainDevice);
IMPORT(clReleaseDevice);
IMPORT(clSetDefaultDeviceCommandQueue);
IMPORT(clGetDeviceAndHostTimer);
IMPORT(clGetHostTimer);
IMPORT(clCreateContext);
IMPORT(clCreateContextFromType);
IMPORT(clRetainContext);
IMPORT(clReleaseContext);
IMPORT(clGetContextInfo);
IMPORT(clSetContextDestructorCallback);
IMPORT(clCreateCommandQueueWithProperties);
IMPORT(clRetainCommandQueue);
IMPORT(clReleaseCommandQueue);
IMPORT(clGetCommandQueueInfo);
IMPORT(clCreateBuffer);
IMPORT(clCreateSubBuffer);
IMPORT(clCreateImage);
IMPORT(clCreatePipe);
IMPORT(clCreateBufferWithProperties);
IMPORT(clCreateImageWithProperties);
IMPORT(clRetainMemObject);
IMPORT(clReleaseMemObject);
IMPORT(clGetSupportedImageFormats);
IMPORT(clGetMemObjectInfo);
IMPORT(clGetImageInfo);
IMPORT(clGetPipeInfo);
IMPORT(clSetMemObjectDestructorCallback);
IMPORT(clSVMAlloc);
IMPORT(clSVMFree);
IMPORT(clCreateSamplerWithProperties);
IMPORT(clRetainSampler);
IMPORT(clReleaseSampler);
IMPORT(clCreateProgramWithSource);
IMPORT(clCreateProgramWithBinary);
IMPORT(clCreateProgramWithBuiltInKernels);
IMPORT(clCreateProgramWithIL);
IMPORT(clRetainProgram);
IMPORT(clReleaseProgram);
IMPORT(clBuildProgram);
IMPORT(clCompileProgram);
IMPORT(clLinkProgram);
IMPORT(clSetProgramReleaseCallback);
IMPORT(clSetProgramSpecializationConstant);
IMPORT(clUnloadPlatformCompiler);
IMPORT(clGetProgramInfo);
IMPORT(clGetProgramBuildInfo);
IMPORT(clCreateKernel);
IMPORT(clCreateKernelsInProgram);
IMPORT(clCloneKernel);
IMPORT(clRetainKernel);
IMPORT(clReleaseKernel);
IMPORT(clSetKernelArg);
IMPORT(clSetKernelArgSVMPointer);
IMPORT(clSetKernelExecInfo);
IMPORT(clGetKernelInfo);
IMPORT(clGetKernelArgInfo);
IMPORT(clGetKernelWorkGroupInfo);
IMPORT(clGetKernelSubGroupInfo);
IMPORT(clWaitForEvents);
IMPORT(clGetEventInfo);
IMPORT(clCreateUserEvent);
IMPORT(clRetainEvent);
IMPORT(clReleaseEvent);
IMPORT(clSetUserEventStatus);
IMPORT(clSetEventCallback);
IMPORT(clGetEventProfilingInfo);
IMPORT(clFlush);
IMPORT(clFinish);
IMPORT(clEnqueueReadBuffer);
IMPORT(clEnqueueReadBufferRect);
IMPORT(clEnqueueWriteBuffer);
IMPORT(clEnqueueWriteBufferRect);
IMPORT(clEnqueueFillBuffer);
IMPORT(clEnqueueCopyBuffer);
IMPORT(clEnqueueCopyBufferRect);
IMPORT(clEnqueueReadImage);
IMPORT(clEnqueueWriteImage);
IMPORT(clEnqueueFillImage);
IMPORT(clEnqueueCopyImage);
IMPORT(clEnqueueCopyImageToBuffer);
IMPORT(clEnqueueCopyBufferToImage);
IMPORT(clEnqueueMapBuffer);
IMPORT(clEnqueueMapImage);
IMPORT(clEnqueueUnmapMemObject);
IMPORT(clEnqueueMigrateMemObjects);
IMPORT(clEnqueueNDRangeKernel);
IMPORT(clEnqueueNativeKernel);
IMPORT(clEnqueueMarkerWithWaitList);
IMPORT(clEnqueueBarrierWithWaitList);
IMPORT(clEnqueueSVMFree);
IMPORT(clEnqueueSVMMemcpy);
IMPORT(clEnqueueSVMMemFill);
IMPORT(clEnqueueSVMMap);
IMPORT(clEnqueueSVMUnmap);
IMPORT(clEnqueueSVMMigrateMem);
IMPORT(clCreateImage2D);
IMPORT(clCreateImage3D);
IMPORT(clEnqueueMarker);
IMPORT(clEnqueueWaitForEvents);
IMPORT(clEnqueueBarrier);
IMPORT(clUnloadCompiler);
//IMPORT(clGetExtensionFunctionAddress);
IMPORT(clCreateCommandQueue);
IMPORT(clCreateSampler);
IMPORT(clEnqueueTask);

///ok program flow:
///make program from source
///build program
///extra program binary and save it to disk

///pt 2: make program from binary
///extract kernels from program
///query kernel args with querying info

///or pt 3: make program from source
///extract kernels from program
///query kernel args

struct arg_info
{
    cl_kernel_arg_type_qualifier qual = 0;
};

std::string get_native_kernel_name(cl_kernel kern)
{
    size_t length;
    clGetKernelInfo_ptr(kern, CL_KERNEL_FUNCTION_NAME, 0, nullptr, &length);

    std::string str;
    str.resize(length);

    clGetKernelInfo_ptr(kern, CL_KERNEL_FUNCTION_NAME, length, str.data(), nullptr);

    return str;
}

struct kernel_data
{
    std::vector<arg_info> args;
    std::string name;

    void load_from_kernel(cl_kernel kern)
    {
        args.clear();

        cl_uint argc = 0;
        clGetKernelInfo_ptr(kern, CL_KERNEL_NUM_ARGS, sizeof(cl_uint), &argc, nullptr);

        for(cl_uint i=0; i < argc; i++)
        {
            arg_info inf;
            clGetKernelArgInfo_ptr(kern, i, CL_KERNEL_ARG_TYPE_QUALIFIER, sizeof(inf.qual), &inf.qual, nullptr);

            args.push_back(inf);
        }

        name = get_native_kernel_name(kern);
    }

    void from_json(nlohmann::json js)
    {
        args.clear();

        name = js["name"];

        std::vector<nlohmann::json> args_js = js["args"];

        for(auto& i : args_js)
        {
            arg_info inf;
            inf.qual = i["qual"];

            args.push_back(inf);
        }
    }

    nlohmann::json to_json()
    {
        std::vector<nlohmann::json> js;

        for(auto& i : args)
        {
            nlohmann::json obj;
            obj["qual"] = i.qual;

            js.push_back(obj);
        }

        nlohmann::json out;
        out["args"] = js;
        out["name"] = name;

        return out;
    }
};

struct _cl_kernel : ref_counting
{
    void* ptr = nullptr;
    kernel_data data;
    bool built_from_source = false;
};

struct _cl_program : ref_counting
{
    void* ptr = nullptr;

    bool built_with_program_info = false;
    bool built_from_source = false;

    std::map<std::string, kernel_data> kernels;

    void kernels_from_json(std::map<std::string, nlohmann::json> data)
    {
        kernels.clear();

        for(auto& i : data)
        {
            kernel_data kdata;
            kdata.from_json(i.second);

            kernels[kdata.name] = kdata;
        }
    }

    nlohmann::json kernels_to_json()
    {
        std::map<std::string, nlohmann::json> js;

        for(auto& i : kernels)
        {
            js[i.first] = i.second.to_json();
        }

        return js;
    }

    void set_kernel_data(const std::map<std::string, kernel_data>& in)
    {
        kernels = in;
    }

    std::vector<std::vector<uint8_t>> to_raw_binary()
    {
        cl_uint num = 0;
        clGetProgramInfo_ptr((cl_program)ptr, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &num, nullptr);

        std::vector<std::vector<uint8_t>> data;
        data.resize(num);

        std::vector<size_t> sizes;
        sizes.resize(num);

        clGetProgramInfo_ptr((cl_program)ptr, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * num, sizes.data(), nullptr);

        for(int i=0; i < num; i++)
        {
            data[i].resize(sizes[i]);
        }

        std::vector<unsigned char*> vector_of_pointers;

        for(auto& i : data)
        {
            vector_of_pointers.push_back(i.data());
        }

        clGetProgramInfo_ptr((cl_program)ptr, CL_PROGRAM_BINARIES, sizeof(unsigned char*) * num, vector_of_pointers.data(), nullptr);

        return data;
    }

    std::vector<std::vector<uint8_t>> to_processed_binary()
    {
        std::vector<std::vector<uint8_t>> raw = to_raw_binary();

        std::vector<std::vector<uint8_t>> cb;

        for(auto& i : raw)
        {
            nlohmann::json data;
            data["binary"] = nlohmann::json::binary_t(i);
            data["kernels"] = kernels_to_json();

            cb.push_back(nlohmann::json::to_cbor(data));
        }

        return cb;
    }

    std::map<std::string, kernel_data> load_kernel_arg_data()
    {
        std::map<std::string, kernel_data> kernels;

        cl_uint kernel_count = 0;

        clCreateKernelsInProgram_ptr((_cl_program*)ptr, 0, nullptr, &kernel_count);

        if(kernel_count == 0)
            return {};

        std::vector<cl_kernel> kerns;

        kerns.resize(kernel_count);

        clCreateKernelsInProgram_ptr((_cl_program*)ptr, kerns.size(), kerns.data(), nullptr);

        for(auto& i : kerns)
        {
            kernel_data dat;
            dat.load_from_kernel(i);

            kernels[dat.name] = dat;

            clReleaseKernel_ptr(i);
        }

        return kernels;
    }
};

template<typename R, typename... Args>
auto detect_args(R(*func)(Args...)){return std::tuple<Args...>();}

template<typename R, typename... Args>
auto detect_return(R(*func)(Args...)){return R();}

_cl_kernel* to_native_type(_cl_kernel* in)
{
    assert(in->ptr);

    return reinterpret_cast<_cl_kernel*>(in->ptr);
}

_cl_program* to_native_type(_cl_program* in)
{
    assert(in->ptr);

    return reinterpret_cast<_cl_program*>(in->ptr);
}

template<typename T>
auto from_native_type(T&& in)
{
    static_assert(!std::is_same_v<T, cl_kernel> && !std::is_same_v<T, cl_program>);

    return in;
}

template<typename Func, typename... T>
auto call(Func f, T&&... args)
{
    if constexpr(std::is_same_v<decltype(detect_return(f)), void>)
        return f(to_native_type(args)...);
    else
        return from_native_type(f(to_native_type(args)...));
}

cl_kernel make_from_native_kernel(cl_program prog, void* in)
{
    std::string native_name = get_native_kernel_name((cl_kernel)in);

    _cl_kernel* ptr = new _cl_kernel();
    ptr->ptr = in;
    ptr->inc();
    ptr->built_from_source = prog->built_from_source;

    if(auto it = prog->kernels.find(native_name); it != prog->kernels.end())
    {
        ptr->data = it->second;
    }

    return ptr;
}

cl_program make_from_native_program(void* in)
{
    _cl_program* ptr = new _cl_program();
    ptr->ptr = in;
    ptr->inc();
    return ptr;
}

cl_int clRetainKernel(cl_kernel kern)
{
    if(kern == nullptr)
        return CL_INVALID_KERNEL;

    kern->inc();
    return call(clRetainKernel_ptr, kern);
}

cl_int clReleaseKernel(cl_kernel kern)
{
    if(kern == nullptr)
        return CL_INVALID_KERNEL;

    bool should_delete = kern->dec();
    cl_int val = call(clReleaseKernel_ptr, kern);

    if(should_delete)
        delete kern;

    return val;
}

cl_int clCreateKernelsInProgram(cl_program program, cl_uint num_kernels, cl_kernel* kernels, cl_uint* num_kernels_ret)
{
    cl_int err = clCreateKernelsInProgram_ptr(to_native_type(program), num_kernels, kernels, num_kernels_ret);

    if(num_kernels != 0 && kernels)
    {
        for(cl_uint i=0; i < num_kernels; i++)
        {
            kernels[i] = make_from_native_kernel(program, kernels[i]);
        }
    }

    return err;
}

cl_kernel clCreateKernel(cl_program program, const char* kernel_name, cl_int* errcode_ret)
{
    return make_from_native_kernel(program, clCreateKernel_ptr(to_native_type(program), kernel_name, errcode_ret));
}

cl_int clRetainProgram(cl_program program)
{
    if(program == nullptr)
        return CL_INVALID_PROGRAM;

    program->inc();
    return call(clRetainProgram_ptr, program);
}

cl_int clReleaseProgram(cl_program program)
{
    if(program == nullptr)
        return CL_INVALID_PROGRAM;

    bool should_delete = program->dec();
    cl_int val = call(clReleaseProgram_ptr, program);

    if(should_delete)
        delete program;

    return val;
}

cl_program clCreateProgramWithSource(cl_context ctx, cl_uint count, const char** strings, const size_t* lengths, cl_int* errcode_ret)
{
    cl_program prog = make_from_native_program(clCreateProgramWithSource_ptr(ctx, count, strings, lengths, errcode_ret));

    prog->built_from_source = true;

    return prog;
}

cl_program clCreateProgramWithBinary(cl_context ctx, cl_uint num_devices, const cl_device_id* device_list, const size_t* lengths, const unsigned char** binaries, cl_int* binary_status, cl_int* errcode_ret)
{
    std::vector<std::vector<uint8_t>> raw_binaries;
    nlohmann::json kernels;

    for(cl_uint i=0; i < num_devices; i++)
    {
        std::vector<uint8_t> data(binaries[i], binaries[i] + lengths[i]);

        try
        {
            nlohmann::json cb = nlohmann::json::from_cbor(data);

            raw_binaries.push_back(cb["binary"].get_binary());

            ///its the same for all of them
            kernels = cb["kernels"];
        }
        catch(...)
        {
            if(binary_status)
                binary_status[i] = CL_INVALID_BINARY;
        }
    }

    std::vector<uint8_t*> ptrs;
    std::vector<size_t> real_lengths;

    for(auto& i : raw_binaries)
    {
        ptrs.push_back(i.data());
        real_lengths.push_back(i.size());
    }

    cl_program prog = make_from_native_program(clCreateProgramWithBinary_ptr(ctx, num_devices, device_list, real_lengths.data(), (const unsigned char**)ptrs.data(), binary_status, errcode_ret));

    prog->kernels_from_json(kernels);

    return prog;
}

cl_int clBuildProgram(cl_program program, cl_uint num_devices, const cl_device_id* device_list, const char* options, void (CL_CALLBACK *  pfn_notify)(cl_program program, void * user_data), void* user_data)
{
    cl_int ret = call(clBuildProgram_ptr, program, num_devices, device_list, options, pfn_notify, user_data);

    if(options)
    {
        std::string opt(options);

        if(opt.contains("-cl-kernel-arg-info"))
        {
            program->built_with_program_info = true;
        }
    }

    if(program->built_from_source && program->built_with_program_info)
        program->set_kernel_data(program->load_kernel_arg_data());

    return ret;
}

cl_int clGetProgramInfo(cl_program program, cl_program_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    if(param_name == CL_PROGRAM_BINARY_SIZES)
    {
        auto bin = program->to_processed_binary();

        std::vector<size_t> sizes;

        for(auto& i : bin)
        {
            sizes.push_back(i.size());
        }

        if(param_value_size_ret)
        {
            *param_value_size_ret = sizeof(size_t) * bin.size();
        }

        if(param_value)
        {
            size_t count = param_value_size / sizeof(size_t);

            size_t* ptr = (size_t*)param_value;

            for(size_t i=0; i < count && i < sizes.size(); i++)
            {
                ptr[i] = sizes.at(i);
            }
        }

        return CL_SUCCESS;
    }

    if(param_name == CL_PROGRAM_BINARIES)
    {
        auto bin = program->to_processed_binary();

        if(param_value_size_ret)
        {
            *param_value_size_ret = sizeof(char*) * bin.size();
        }

        if(param_value)
        {
            char** pointers = (char**)param_value;

            size_t count = param_value_size / sizeof(char*);

            for(size_t i=0; i < count && i < bin.size(); i++)
            {
                char* to_write = pointers[i];

                for(size_t kk=0; kk < bin[i].size(); kk++)
                {
                    to_write[kk] = bin[i][kk];
                }
            }
        }

        return CL_SUCCESS;
    }

    return clGetProgramInfo_ptr(to_native_type(program), param_name, param_value_size, param_value, param_value_size_ret);
}

cl_int clGetKernelArgInfo(cl_kernel kernel, cl_uint arg_index, cl_kernel_arg_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    if(kernel->built_from_source)
    {
        return call(clGetKernelArgInfo_ptr, kernel, arg_index, param_name, param_value_size, param_value, param_value_size_ret);
    }
    else
    {
        if(param_name == CL_KERNEL_ARG_TYPE_QUALIFIER)
        {
            if(param_value_size_ret)
                *param_value_size_ret = sizeof(cl_kernel_arg_type_qualifier);

            if(param_value)
            {
                if(param_value_size != sizeof(cl_kernel_arg_type_qualifier))
                    return CL_INVALID_VALUE;

                if(arg_index < kernel->data.args.size())
                {
                    *(size_t*)param_value = kernel->data.args[arg_index].qual;
                }
                else
                {
                    return CL_INVALID_ARG_INDEX;
                }
            }

            return CL_SUCCESS;
        }
    }

    return CL_KERNEL_ARG_INFO_NOT_AVAILABLE;
}

#define NAME_TYPE(name, idx) std::remove_cvref_t<decltype(std::get<idx>(detect_args(name##_ptr)))>
#define NAME_RETURN(name) std::remove_cvref_t<decltype(detect_return(name##_ptr))>

#define SHIM_0(name) auto name(void) -> NAME_RETURN(name) {return call(name##_ptr);}
#define SHIM_1(name) auto name(NAME_TYPE(name, 0) a0) -> NAME_RETURN(name) {return call(name##_ptr, a0);}
#define SHIM_2(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1);}
#define SHIM_3(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2);}
#define SHIM_4(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3);}
#define SHIM_5(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4);}
#define SHIM_6(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5);}
#define SHIM_7(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6);}
#define SHIM_8(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7);}
#define SHIM_9(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8);}
#define SHIM_10(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);}
#define SHIM_11(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);}
#define SHIM_12(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);}
#define SHIM_13(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11, NAME_TYPE(name, 12) a12) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);}
#define SHIM_14(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11, NAME_TYPE(name, 12) a12, NAME_TYPE(name, 13) a13) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);}
#define SHIM_15(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8, NAME_TYPE(name, 9) a9, NAME_TYPE(name, 10) a10, NAME_TYPE(name, 11) a11, NAME_TYPE(name, 12) a12, NAME_TYPE(name, 13) a13, NAME_TYPE(name, 14) a14) -> NAME_RETURN(name) {return call(name##_ptr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);}

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
//SHIM_5(clCreateProgramWithSource);
//SHIM_7(clCreateProgramWithBinary);
//SHIM_5(clCreateProgramWithBuiltInKernels);
//SHIM_4(clCreateProgramWithIL);
//SHIM_1(clRetainProgram);
//SHIM_1(clReleaseProgram);
//SHIM_6(clBuildProgram);
//SHIM_9(clCompileProgram); //TODO: LINK PROGRAM
//SHIM_9(clLinkProgram); //TODO: LINK PROGRAM
SHIM_3(clSetProgramReleaseCallback);
SHIM_4(clSetProgramSpecializationConstant);
SHIM_1(clUnloadPlatformCompiler);
//SHIM_5(clGetProgramInfo);
SHIM_6(clGetProgramBuildInfo);
//SHIM_3(clCreateKernel);
//SHIM_4(clCreateKernelsInProgram);
//SHIM_2(clCloneKernel);
//SHIM_1(clRetainKernel);
//SHIM_1(clReleaseKernel);
SHIM_4(clSetKernelArg);
SHIM_3(clSetKernelArgSVMPointer);
SHIM_4(clSetKernelExecInfo);
SHIM_5(clGetKernelInfo);
//SHIM_6(clGetKernelArgInfo);
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

