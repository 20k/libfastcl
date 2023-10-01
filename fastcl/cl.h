#ifndef CL_H_INCLUDED
#define CL_H_INCLUDED

#include <CL/cl.h>

#define CL_QUEUE_MULTITHREADED (1 << 9)

void clBeginSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue);
void clEndSpliceEx(cl_command_queue real_queue, cl_command_queue c_pqueue);

#endif // CL_H_INCLUDED
