#pragma once
#include <sys/mman.h>
#include <algorithm>
#include <core/state.h>
#include <array>

#include <loader/mach_object.h>
#include <ops/base_op.h>

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
        void* _rodata;
        void* _data;
        void* _instructions;
        std::shared_ptr<State> _state;
    public: 
        Session(std::string_view filepath) : _state(std::make_shared<State>(filepath)) {
            // TODO: proper stack initialization
            _state->get_r_ref_64(31) = 512 * 1024 - 1; 
            auto file = loader::MachObject::load(filepath);
            auto [instrs, off] = file.load_instructions();
            assert(instrs != nullptr);
            _state->update_pc(off);
            while (ops::handle_instruction(instrs[_state->get_pc()], *_state)) 
                _state->advance_pc();
        }

        std::shared_ptr<State> get_state() {
            return _state;
        }
    };
}