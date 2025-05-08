#pragma once
#include <sys/mman.h>
#include <algorithm>
#include <core/state.h>
#include <array>

#include <session/execution_unit.h>

#include <loader/mach_object.h>


/*

    What should a session have? 
    - Loader (link commands + segments ptr)
    - CPU state (for 1 core and 1 thread) - 1 set of registers, 1 stack 
    - Execution unit 
    - Memory emulation (for dynamic allocations)
    - Execution unit (gets 32-bit instruction, processes, updates state)
    - ROM (rodata) (from the binary, dynamic allocated at the start, has fixed size)
    - DATA segment (global data, dynamicaly allocated at the start)
    - MMU (loads, stores)
    - PFU (instructions prefetcher)
      (just emulation, has madvise(MADV_WILLNEED|MADV_SEQUENTIAL) on the mapped file under the hood), 

    Basic pipeline: 
    1. Create a session with filepath (TODO pass args), 
    2. Load an executable by the loader (just TEXT (i + d), DATA segments for now), 
    3. Create the State with a stack, store DATA and TEXT rodata inside preallocated buffers, TEXT instructions are mapped
    4. Prefetch mmaped instruction
    
*/
 
namespace cheeky::session {
    class Session {
        using State = core::State;
    private:
        char* _stack;
        void* _rodata;
        void* _data;
        void* _instructions;
        std::shared_ptr<State> _state = std::make_shared<State>();
        session::ExecutionUnit _exec;
    public: 
        Session(std::string_view filepath) : _exec(_state) {
            auto file = loader::MachObject::load(filepath);
            auto instrs = file.load_instructions();
            assert(instrs != nullptr);
            _exec.execute(*instrs);
        }
    };
}