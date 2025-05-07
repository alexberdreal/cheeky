#pragma once
#include <sys/mman.h>
#include <algorithm>
#include <state.h>
#include <array>

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
 
const size_t MAX_INSTR_PREFETCH_SIZE = 4 * 1024;  // 4KB 

void prefetch_instructions(void* addr, size_t size) {
    madvise(addr, std::max(size, MAX_INSTR_PREFETCH_SIZE), MADV_WILLNEED | MADV_SEQUENTIAL);
}

namespace cheeky::session {

    class Session {
        private:
            ops::State _state;
            char* _stack;
            void* _rodata;
            void* _data;
            void* _instructions;
    };
}