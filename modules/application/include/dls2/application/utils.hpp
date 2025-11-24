#include <iostream>

extern "C"
{
#include <proc/readproc.h>
}

using namespace dls;

inline void showProcessInfo()
{
    proc_t proc_info;
    look_up_our_self(&proc_info);
    // look_up_process(pid, &proc_info); // or readproc()
    std::cout << "CPU: " << proc_info.pcpu << ", RSS: " << proc_info.resident << " KB\n";
}