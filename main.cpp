#include <mach/mach.h>  // Main Mach API header
#include <mach/task.h>  // Task-related functions
#include <mach/thread_act.h>  // Thread function
#include <loader/mach_object.h>
#include <iostream>
#include <assert.h>

int main(int argc, char** argv) {
    assert(argc == 2);

    auto exec_path = argv[1];
    auto object = cheekydbg::loader::MachObject::load(exec_path);
    task_t new_task;
    auto res = task_create(mach_task_self(), nullptr, 0, false, &new_task);
    vm_address_t addr = 0;
    vm_allocate(new_task, &addr, 8 * 1024 * 1024 * 1024, 0);
    vm_map_64()

    // operands = 2, add 

    processor_set_tasks
    thread_create
    assert(res == 0);
    arm_thread_state64_t st;   
    // What do I need? PC = FP, SP 
    st.__pc = 

    return 0;
}
