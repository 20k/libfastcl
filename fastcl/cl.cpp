#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define CL_USE_DEPRECATED_OPENCL_2_1_APIS
#define CL_USE_DEPRECATED_OPENCL_2_2_APIS

#include <CL/cl.h>
#include <CL/cl_gl.h>

#define CL_QUEUE_MULTITHREADED (1 << 9)

#include <nlohmann/json.hpp>
#include <iostream>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

template<typename T>
auto to_native_type(T&& in)
{
    return std::forward<T>(in);
}

_cl_kernel* to_native_type(_cl_kernel* in);
_cl_program* to_native_type(_cl_program* in);
_cl_command_queue* to_native_type(_cl_command_queue* in);

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

#if 0
///each copy of this is unique. We don't copy these, we take pointers
///that renders the entire ref counting mechanism basically useless, right?
template<typename T>
struct ref_counted
{
    std::atomic_uint cnt{0};
    boost::intrusive_ptr<T> ptr;

    /*explicit ref_counted(T* in) : ptr(in, false)
    {
    }*/

    ///ptr == nullptr after this
    T* release_native()
    {
        return ptr.detach();
    }

    friend void intrusive_ptr_add_ref(ref_counted<T>* in)
    {
        in->cnt++;
    }

    friend void intrusive_ptr_release(ref_counted<T>* in)
    {
        if((--in->cnt) == 0)
        {
            std::cout << "Delete\n";
            delete in;
        }
    }
};
#endif

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
#ifdef CL_VERSION_3_0
IMPORT(clSetContextDestructorCallback);
#endif
IMPORT(clCreateCommandQueueWithProperties);
IMPORT(clRetainCommandQueue);
IMPORT(clReleaseCommandQueue);
IMPORT(clGetCommandQueueInfo);
IMPORT(clCreateBuffer);
IMPORT(clCreateSubBuffer);
IMPORT(clCreateImage);
IMPORT(clCreatePipe);
#ifdef CL_VERSION_3_0
IMPORT(clCreateBufferWithProperties);
IMPORT(clCreateImageWithProperties);
#endif
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

IMPORT(clCreateFromGLTexture);
IMPORT(clEnqueueAcquireGLObjects);
IMPORT(clEnqueueReleaseGLObjects);

bool is_event_finished(cl_event evt)
{
    if(evt == nullptr)
        return true;

    cl_int status = 0;

    if(clGetEventInfo_ptr(evt, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), (void*)&status, nullptr) != CL_SUCCESS)
        assert(false);

    return status == CL_COMPLETE;
}

cl_mem_flags get_flags(cl_mem in)
{
    cl_mem_flags ret = 0;
    clGetMemObjectInfo_ptr(in, CL_MEM_FLAGS, sizeof(cl_mem_flags), &ret, nullptr);

    return ret;
}

///does not retain
std::optional<cl_mem> get_parent(cl_mem in)
{
    cl_mem ret;
    ///unclear what this does to the reference count
    clGetMemObjectInfo_ptr(in, CL_MEM_ASSOCIATED_MEMOBJECT, sizeof(cl_mem), &ret, nullptr);

    if(ret == nullptr)
        return std::nullopt;

    return ret;
}

///does not retain
std::pair<cl_mem, cl_mem_flags> get_barrier_vars(cl_mem in)
{
    assert(in);

    std::optional<cl_mem> parent = get_parent(in);

    while(parent.has_value())
    {
        auto next_parent_opt = get_parent(parent.value());

        if(next_parent_opt.has_value())
            parent = next_parent_opt;
        else
            break;
    }

    cl_mem_flags flags = get_flags(in);

    cl_mem val = parent.value_or(in);

    return {val, flags};
}

struct access_storage
{
    std::map<cl_mem, std::vector<cl_mem_flags>> store;
    bool is_barrier = false;

    void add(bool manual_read_only, cl_mem in)
    {
        assert(in);

        auto vars = get_barrier_vars(in);

        clRetainMemObject_ptr(vars.first);

        if(manual_read_only)
            store[vars.first].push_back(CL_MEM_READ_ONLY);
        else
            store[vars.first].push_back(vars.second);
    }

    /*void remove(cl_mem in)
    {
        auto it = store.find(in);

        if(it == store.end())
            return;

        clReleaseMemObject_ptr(in);
        store.erase(it);
    }*/

    void remove_all()
    {
        for(auto& i : store)
        {
            clReleaseMemObject_ptr(i.first);
        }

        store.clear();
    }
};

namespace
{
bool requires_memory_barrier_raw(cl_mem_flags flag1, cl_mem_flags flag2)
{
    ///do not need a memory barrier between two overlapping objects if and only if they're both read only
    if((flag1 & CL_MEM_READ_ONLY) && (flag2 & CL_MEM_READ_ONLY))
        return false;

    //if((flag1 & CL_MEM_WRITE_ONLY) && (flag2 & CL_MEM_WRITE_ONLY))
    //    return false;

    return true;
}

bool requires_memory_barrier(const access_storage& base, const access_storage& theirs)
{
    if(base.is_barrier || theirs.is_barrier)
        return true;

    for(auto& [mem, flags] : theirs.store)
    {
        auto it = base.store.find(mem);

        if(it == base.store.end())
            continue;

        for(cl_mem_flags f : it->second)
        {
            for(cl_mem_flags g : flags)
            {
                if(requires_memory_barrier_raw(f, g))
                    return true;
            }
        }
    }

    return false;
}
}

struct _cl_command_queue : ref_counting
{
    cl_command_queue accessory;
    std::vector<cl_command_queue> queues;
    int which_queue = 0;
    bool is_managed_queue = false;
    bool in_fallback_mode = false;

    std::vector<std::tuple<cl_event, access_storage, std::string>> event_history;

    void make_raw(cl_context ctx, cl_device_id device, const std::vector<cl_queue_properties>& properties, cl_int* errcode_ret)
    {
        is_managed_queue = false;

        auto props = properties;
        props.push_back(0);

        accessory = clCreateCommandQueueWithProperties_ptr(ctx, device, props.data(), errcode_ret);

        assert(*errcode_ret == CL_SUCCESS);
    }

    void make_managed(cl_context ctx, cl_device_id device, const std::vector<cl_queue_properties>& properties, cl_int* errcode_ret)
    {
        is_managed_queue = true;

        cl_command_queue_properties props[] = {0};

        ///out of order queue?
        accessory = clCreateCommandQueueWithProperties_ptr(ctx, device, props, errcode_ret);

        assert(*errcode_ret == CL_SUCCESS);

        int max_queues = 16;

        for(int i=0; i < max_queues; i++)
        {
            queues.push_back(clCreateCommandQueueWithProperties_ptr(ctx, device, props, errcode_ret));

            assert(*errcode_ret == CL_SUCCESS);
        }
    }

    cl_command_queue next()
    {
        cl_command_queue q = queues[which_queue];
        which_queue++;
        which_queue %= queues.size();
        return q;
    }
};

void cleanup_events(_cl_command_queue& pqueue);

_cl_command_queue* to_native_type(_cl_command_queue* in)
{
    if(!in->is_managed_queue)
    {
        return in->accessory;
    }
    else
    {
        cleanup_events(*in);

        std::vector<cl_event> all_events;

        for(auto& i : in->event_history)
        {
            all_events.push_back(std::get<0>(i));
        }

        ///enqueue a marker on the 0th queue that synchronises all other queues, so that we can dump work into the 0th queue again
        ///this assumes an in order queue
        cl_event evt = nullptr;
        clEnqueueMarkerWithWaitList_ptr(in->queues[0], all_events.size(), all_events.data(), &evt);

        assert(evt);

        access_storage store;
        store.is_barrier = true;

        in->event_history.push_back({evt, store, "compat"});

        return in->queues[0];
    }
}

namespace
{
std::vector<cl_event> get_implicit_dependencies(_cl_command_queue& pqueue, const access_storage& store)
{
    std::vector<cl_event> deps;

    for(const auto& [evt, args, tag] : pqueue.event_history)
    {
        if(requires_memory_barrier(store, args))
        {
            deps.push_back(evt);
        }
    }

    return deps;
}

std::vector<cl_event> get_implicit_dependencies(_cl_command_queue& pqueue, cl_mem obj)
{
    access_storage store;
    store.add(false, obj);

    auto deps = get_implicit_dependencies(pqueue, store);

    store.remove_all();

    return deps;
}
}

/*
template<typename T>
cl::event add(const T& func, cl::mem_object& obj, const std::vector<cl::event>& events)
{
    cleanup_events();

    std::vector<cl::event> evts = get_dependencies(obj);

    evts.insert(evts.end(), events.begin(), events.end());

    cl::command_queue& exec_on = mqueue.next();

    cl::event next = func(exec_on, evts);

    cl::access_storage store;
    store.add(obj);

    event_history.push_back({next, store, "generic"});

    return next;
}*/

///all *enqueue* commands, all flush and finish commands
///clgetcommandqueueinfo

///cleanup_events
/*for(int i=0; i < (int)event_history.size(); i++)
{
    cl::event& test = std::get<0>(event_history[i]);

    if(test.is_finished())
    {
        event_history.erase(event_history.begin() + i);
        i--;
        continue;
    }
}*/

void cleanup_events(_cl_command_queue& pqueue)
{
    for(int i=0; i < (int)pqueue.event_history.size(); i++)
    {
        cl_event& test = std::get<0>(pqueue.event_history[i]);

        if(is_event_finished(test))
        {
            auto [found_event, found_store, unused] = pqueue.event_history[i];

            clReleaseEvent_ptr(found_event);
            found_store.remove_all();

            pqueue.event_history.erase(pqueue.event_history.begin() + i);
            i--;
            continue;
        }
    }
}

template<typename T>
auto add_single(_cl_command_queue& pqueue, T&& func, cl_mem obj, const std::vector<cl_event>& events, cl_event* external_event)
{
    if(!pqueue.is_managed_queue)
    {
        cl_event evt = nullptr;
        auto result = func(pqueue.accessory, events, evt);

        if(external_event)
            *external_event = evt;
        else
            clReleaseEvent_ptr(evt);

        return result;
    }

    cleanup_events(pqueue);

    std::vector<cl_event> evts = get_implicit_dependencies(pqueue, obj);

    evts.insert(evts.end(), events.begin(), events.end());

    cl_command_queue exec_on = pqueue.next();
    //auto exec_on = pqueue.queues[0];

    cl_event next = nullptr;

    auto result = func(exec_on, evts, next);

    assert(next);

    access_storage store;
    store.add(false, obj);

    clRetainEvent_ptr(next);
    pqueue.event_history.push_back({next, store, "generic"});

    if(external_event)
        *external_event = next;

    return result;
}

std::vector<cl_event> make_events(cl_int num, const cl_event* event)
{
    if(num == 0)
        return {};

    if(event == nullptr)
        return {};

    std::vector<cl_event> ret;

    for(int i=0; i < num; i++)
    {
        ret.push_back(event[i]);
    }

    return ret;
}

cl_int clEnqueueReadBuffer(cl_command_queue pqueue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    auto native_events = make_events(num_events_in_wait_list, event_wait_list);

    return add_single(*pqueue, [&](cl_command_queue native_queue, const std::vector<cl_event>& evts, cl_event& out)
    {
        return clEnqueueReadBuffer_ptr(native_queue, buffer, blocking_read, offset, size, ptr, evts.size(), evts.data(), &out);
    }, buffer, native_events, event);
}

cl_int clEnqueueWriteBuffer(cl_command_queue pqueue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t size, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    auto native_events = make_events(num_events_in_wait_list, event_wait_list);

    return add_single(*pqueue, [&](cl_command_queue native_queue, const std::vector<cl_event>& evts, cl_event& out)
    {
        return clEnqueueWriteBuffer_ptr(native_queue, buffer, blocking_write, offset, size, ptr, evts.size(), evts.data(), &out);
    }, buffer, native_events, event);
}

cl_command_queue clCreateCommandQueueWithProperties(cl_context ctx, cl_device_id device, const cl_queue_properties* properties, cl_int* errcode_ret)
{
    _cl_command_queue* pqueue = new _cl_command_queue;
    pqueue->inc();

    std::vector<cl_queue_properties> props;

    for(int i=0;; i+=2)
    {
        if(properties[i] == 0)
            break;

        props.push_back(properties[i]);
        props.push_back(properties[i+1]);
    }

    bool is_raw = true;

    for(int i=0; i < (int)props.size() - 1; i++)
    {
        if(props[i] == CL_QUEUE_PROPERTIES)
        {
            if(props[i + 1] & CL_QUEUE_MULTITHREADED)
                is_raw = false;
        }
    }

    cl_int err = CL_SUCCESS;

    if(is_raw)
        pqueue->make_raw(ctx, device, props, &err);
    else
        pqueue->make_managed(ctx, device, props, &err);

    if(errcode_ret)
        *errcode_ret = err;

    return reinterpret_cast<cl_command_queue>(pqueue);
}

cl_command_queue clCreateCommandQueue(cl_context ctx, cl_device_id device, cl_command_queue_properties props, cl_int* errcode_ret)
{
    if(props != 0)
    {
        cl_queue_properties compat_props[] = {CL_QUEUE_PROPERTIES, props, 0};

        return clCreateCommandQueueWithProperties(ctx, device, compat_props, errcode_ret);
    }
    else
    {
        cl_queue_properties compat_props[] = {0};

        return clCreateCommandQueueWithProperties(ctx, device, compat_props, errcode_ret);
    }
}

cl_int clRetainCommandQueue(cl_command_queue cqueue)
{
    cqueue->inc();

    if(!cqueue->is_managed_queue)
        return clRetainCommandQueue_ptr(cqueue->accessory);
    else
    {
        clRetainCommandQueue_ptr(cqueue->accessory);

        for(auto& i : cqueue->queues)
            clRetainCommandQueue_ptr(i);

        return CL_SUCCESS;
    }
}

cl_int clReleaseCommandQueue(cl_command_queue cqueue)
{
    auto decrement_and_delete = [&]()
    {
        if(cqueue->dec())
        {
            delete cqueue;
        }
    };

    if(!cqueue->is_managed_queue)
    {
        cl_int result = clReleaseCommandQueue_ptr(cqueue->accessory);

        decrement_and_delete();

        return result;
    }
    else
    {
        clReleaseCommandQueue_ptr(cqueue->accessory);

        for(auto& i : cqueue->queues)
            clReleaseCommandQueue_ptr(i);

        decrement_and_delete();

        return CL_SUCCESS;
    }
}

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

    ///std::map<mem_object, std::vector<cl_mem_flags>> store;
    std::map<cl_uint, std::pair<bool, cl_mem>> args;
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

        for(int i=0; i < (int)num; i++)
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

void clBeginSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue)
{
    if(!real_queue->is_managed_queue && !c_pqueue->is_managed_queue)
    {
        cl_event evt = nullptr;
        clEnqueueMarkerWithWaitList_ptr(real_queue->accessory, 0, nullptr, &evt);

        clEnqueueMarkerWithWaitList_ptr(c_pqueue->accessory, 1, &evt, nullptr);
        clReleaseEvent_ptr(evt);
        return;
    }

    assert(!real_queue->is_managed_queue);
    assert(c_pqueue->is_managed_queue);

    assert(real_queue->accessory);

    cl_event evt = nullptr;
    clEnqueueMarkerWithWaitList_ptr(real_queue->accessory, 0, nullptr, &evt);

    for(cl_command_queue q : c_pqueue->queues)
    {
        clEnqueueMarkerWithWaitList_ptr(q, 1, &evt, nullptr);
    }

    clReleaseEvent_ptr(evt);
}

void clEndSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue)
{
    if(!real_queue->is_managed_queue && !c_pqueue->is_managed_queue)
    {
        cl_event evt = nullptr;
        clEnqueueMarkerWithWaitList_ptr(c_pqueue->accessory, 0, nullptr, &evt);

        clEnqueueMarkerWithWaitList_ptr(real_queue->accessory, 1, &evt, nullptr);
        clReleaseEvent_ptr(evt);
        return;
    }

    assert(!real_queue->is_managed_queue);
    assert(c_pqueue->is_managed_queue);

    std::vector<cl_event> evts;

    for(cl_command_queue q : c_pqueue->queues)
    {
        cl_event evt = nullptr;

        clEnqueueMarkerWithWaitList_ptr(q, 0, nullptr, &evt);

        evts.push_back(evt);
    }

    clEnqueueMarkerWithWaitList_ptr(real_queue->accessory, evts.size(), evts.data(), nullptr);

    for(cl_event e : evts)
    {
        clReleaseEvent_ptr(e);
    }
}

cl_int clEnqueueNDRangeKernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    if(!command_queue->is_managed_queue)
        return clEnqueueNDRangeKernel_ptr(command_queue->accessory, to_native_type(kernel), work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
    //else
    //    return clEnqueueNDRangeKernel_ptr(command_queue->queues[0], to_native_type(kernel), work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);

    cleanup_events(*command_queue);

    access_storage store;

    for(auto& i : kernel->args)
    {
        store.add(i.second.first, i.second.second);
    }

    auto deps = get_implicit_dependencies(*command_queue, store);
    auto converted_deps = make_events(num_events_in_wait_list, event_wait_list);

    deps.insert(deps.end(), converted_deps.begin(), converted_deps.end());

    cl_command_queue exec_on = command_queue->next();
    //cl_command_queue exec_on = command_queue->queues[0];

    cl_event evt = nullptr;

    cl_int ret = clEnqueueNDRangeKernel_ptr(exec_on, to_native_type(kernel), work_dim, global_work_offset, global_work_size, local_work_size, deps.size(), deps.data(), &evt);

    clFlush_ptr(exec_on);

    clRetainEvent_ptr(evt);
    command_queue->event_history.push_back({evt, store, "kernel"});

    if(event)
    {
        *event = evt;
    }

    return ret;
}

///todo: use the kernel access info to generate read/write info
cl_int clSetKernelArgMemEx(cl_kernel kern, cl_uint arg_index, size_t arg_size, const void* arg_value)
{
    bool read_only = false;

    {
        cl_kernel_arg_type_qualifier qual = 0;
        clGetKernelArgInfo(kern, arg_index, CL_KERNEL_ARG_TYPE_QUALIFIER, sizeof(cl_kernel_arg_type_qualifier), &qual, nullptr);

        if(qual & CL_KERNEL_ARG_TYPE_CONST)
            read_only = true;
    }

    kern->args[arg_index] = {read_only, *(cl_mem*)arg_value};

    assert(arg_size == sizeof(cl_mem));

    return call(clSetKernelArg_ptr, kern, arg_index, arg_size, arg_value);
}

cl_int clSetKernelArg(cl_kernel kern, cl_uint arg_index, size_t arg_size, const void* arg_value)
{
    auto it = kern->args.find(arg_index);

    if(it != kern->args.end())
        kern->args.erase(it);

    return call(clSetKernelArg_ptr, kern, arg_index, arg_size, arg_value);
}

cl_int clFlush(cl_command_queue command_queue)
{
    if(!command_queue->is_managed_queue)
        return clFlush_ptr(command_queue->accessory);

    for(auto& i : command_queue->queues)
    {
        clFlush_ptr(i);
    }

    cleanup_events(*command_queue);

    return CL_SUCCESS;
}

cl_int clFinish(cl_command_queue command_queue)
{
    if(!command_queue->is_managed_queue)
        return clFinish_ptr(command_queue->accessory);

    for(auto& i : command_queue->queues)
    {
        clFinish_ptr(i);
    }

    cleanup_events(*command_queue);

    return CL_SUCCESS;
}

cl_int clGetCommandQueueInfo(cl_command_queue command_queue, cl_command_queue_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    if(!command_queue->is_managed_queue)
        return clGetCommandQueueInfo_ptr(command_queue->accessory, param_name, param_value_size, param_value, param_value_size_ret);

    return clGetCommandQueueInfo_ptr(command_queue->queues[0], param_name, param_value_size, param_value, param_value_size_ret);
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
    std::string opt_str;

    if(options)
        opt_str = options;

    opt_str += " -cl-kernel-arg-info";

    cl_int ret = call(clBuildProgram_ptr, program, num_devices, device_list, opt_str.c_str(), pfn_notify, user_data);

    program->built_with_program_info = true;

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
#ifdef CL_VERSION_3_0
SHIM_3(clSetContextDestructorCallback);
#endif
//SHIM_4(clCreateCommandQueueWithProperties);
//SHIM_1(clRetainCommandQueue);
//SHIM_1(clReleaseCommandQueue);
//SHIM_5(clGetCommandQueueInfo);
SHIM_5(clCreateBuffer);
SHIM_5(clCreateSubBuffer);
SHIM_6(clCreateImage);
SHIM_6(clCreatePipe);
#ifdef CL_VERSION_3_0
SHIM_6(clCreateBufferWithProperties);
SHIM_7(clCreateImageWithProperties);
#endif
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
//SHIM_4(clSetKernelArg);
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
//SHIM_1(clFlush);
//SHIM_1(clFinish);
//SHIM_9(clEnqueueReadBuffer);
SHIM_14(clEnqueueReadBufferRect);
//SHIM_9(clEnqueueWriteBuffer);
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
//SHIM_9(clEnqueueNDRangeKernel);
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
//SHIM_4(clCreateCommandQueue);
SHIM_5(clCreateSampler);
SHIM_5(clEnqueueTask);

SHIM_6(clCreateFromGLTexture);
SHIM_6(clEnqueueAcquireGLObjects);
SHIM_6(clEnqueueReleaseGLObjects);
