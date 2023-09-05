#include "cl.h"
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>

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

#define IMPORT(name) auto name##_ptr = get_ptr<decltype(name)>(#name);

IMPORT(clCreateBuffer);
IMPORT(clCreateCommandQueue);
IMPORT(clCreateContext);
IMPORT(clCreateProgramWithSource);

IMPORT(clBuildProgram);
IMPORT(clCompileProgram);
IMPORT(clCreateKernel);
IMPORT(clCreateKernelsInProgram);
IMPORT(clSetKernelArg);
IMPORT(clEnqueueNDRangeKernel);
IMPORT(clFinish);
IMPORT(clEnqueueWriteBuffer);
IMPORT(clEnqueueReadBuffer);

decltype(clCreateBuffer)* clCreateBuffer_decayed = clCreateBuffer_ptr;

template<typename R, typename... Args>
auto detect_args(R(*func)(Args...)){return std::tuple<Args...>();}

template<typename R, typename... Args>
auto detect_return(R(*func)(Args...)){return R();}

static_assert(std::is_same_v<std::remove_cvref_t<decltype(std::get<0>(detect_args(clCreateBuffer_ptr)))>, cl_context>);

#define NAME_TYPE(name, idx) std::remove_cvref_t<decltype(std::get<idx>(detect_args(name##_ptr)))>
#define NAME_RETURN(name) std::remove_cvref_t<decltype(detect_return(name##_ptr))>

#define SHIM_0(name) auto name(void) -> NAME_RETURN(name) {return name##_ptr();}
#define SHIM_1(name) auto name(NAME_TYPE(name, 0) a0) -> NAME_RETURN(name) {return name##_ptr(a0);}
#define SHIM_2(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1) -> NAME_RETURN(name) {return name##_ptr(a0, a1);}
#define SHIM_3(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2);}
#define SHIM_4(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2, a3);}
#define SHIM_5(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2, a3, a4);}
#define SHIM_6(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2, a3, a4, a5);}
#define SHIM_7(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2, a3, a4, a5, a6);}
#define SHIM_8(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2, a3, a4, a5, a6, a7);}
#define SHIM_9(name) auto name(NAME_TYPE(name, 0) a0, NAME_TYPE(name, 1) a1, NAME_TYPE(name, 2) a2, NAME_TYPE(name, 3) a3, NAME_TYPE(name, 4) a4, NAME_TYPE(name, 5) a5, NAME_TYPE(name, 6) a6, NAME_TYPE(name, 7) a7, NAME_TYPE(name, 8) a8) -> NAME_RETURN(name) {return name##_ptr(a0, a1, a2, a3, a4, a5, a6, a7, a8);}

SHIM_5(clCreateBuffer);
