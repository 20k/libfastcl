#ifndef CL_H_INCLUDED
#define CL_H_INCLUDED

#include <CL/cl.h>

#define CL_QUEUE_MULTITHREADED (1 << 9)

cl_command_queue clCreateCommandQueueWithPropertiesEx(cl_context ctx, cl_device_id device, const cl_queue_properties* properties, cl_int* errcode_ret);
cl_int clEnqueueReadBufferEx(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void* ptr, cl_int num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
cl_int clEnqueueWriteBufferEx(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t size, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
cl_command_queue clCreateCommandQueueWithPropertiesEx(cl_context ctx, cl_device_id device, const cl_queue_properties* properties, cl_int* errcode_ret);
cl_command_queue clCreateCommandQueueEx(cl_context ctx, cl_device_id device, cl_command_queue_properties props, cl_int* errcode_ret);
void clBeginSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue);
void clEndSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue);
cl_int clEnqueueNDRangeKernelEx(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
cl_int clSetKernelArgMemEx(cl_kernel kern, cl_uint arg_index, size_t arg_size, const void* arg_value);
cl_int clFinishEx(cl_command_queue command_queue);

#endif // CL_H_INCLUDED
