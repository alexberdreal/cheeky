#include <mach/mach.h>  // Main Mach API header
#include <mach/task.h>  // Task-related functions
#include <mach/thread_act.h>  // Thread function
#include <loader/mach_object.h>
#include <assert.h>
#include <session/session.h>

int main(int argc, char** argv) {
    assert(argc == 2);

    auto exec_path = argv[1];
    
    cheeky::session::Session sesh(argv[1]);

    return 0;
}
