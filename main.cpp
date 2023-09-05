#include <iostream>
#include "fastcl/cl.h"

int main()
{
    cl_context ctx;

    clCreateBuffer(ctx, cl_mem_flags(), 0, nullptr, nullptr);

    return 0;
}
