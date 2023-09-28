#ifndef CL_H_INCLUDED
#define CL_H_INCLUDED

#include <CL/cl.h>

#define CL_QUEUE_MULTITHREADED (1 << 9)

void clBeginSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue);
void clEndSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue);
cl_int clSetKernelArgMemEx(cl_kernel kern, cl_uint arg_index, size_t arg_size, const void* arg_value);

inline
cl_int clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t arg_size, const cl_mem* arg_value)
{
    return clSetKernelArgMemEx(kernel, arg_index, arg_size, arg_value);
}

#endif // CL_H_INCLUDED
