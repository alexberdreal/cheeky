#include <mach/mach.h>  // Main Mach API header
#include <mach/task.h>  // Task-related functions
#include <mach/thread_act.h>  // Thread function
#include <loader/mach_object.h>
#include <iostream>

int main(int argc, char** argv) {
    // get send rights to the task's kernel port 
    // auto self = mach_task_self();
    // uint32_t size = sizeof(task_basic_info);
    // int buffer[sizeof(task_basic_info)];
    // task_info(self, TASK_BASIC_INFO, static_cast<task_info_t>(buffer), static_cast<mach_msg_type_number_t*>(&size));
    // auto casted = reinterpret_cast<task_basic_info*>(buffer);
    // std::cout << "suspends: " << casted->suspend_count << std::endl; 

    auto macho = cheekydbg::loader::MachObject::load("/Users/aberdnikov/cheekydbg/build/test");
    macho.print_internals();

    return 0;
}
