#include <mach/mach.h>  // Main Mach API header
#include <mach/task.h>  // Task-related functions
#include <mach/thread_act.h>  // Thread function
#include <loader/mach_object.h>
#include <assert.h>
#include <session/session.h>
#include <core/logger.h>

int main(int argc, char** argv) {
    assert(argc == 2);

    spdlog::set_level(static_cast<spdlog::level::level_enum>(LOGLEVEL));

    try 
    {
        cheeky::session::Session sesh(argv[1]);
    } 
    catch (const std::runtime_error& ex) 
    {
        cheeky::core::Logger logger {"cheeky::main"};
        logger.error("Caught exception: ", ex.what());
    }

    spdlog::shutdown();
    return 0;
}
