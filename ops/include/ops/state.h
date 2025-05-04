#pragma once
#include <stdint.h>

namespace cheeky {
    struct State {
        /// General-purpose registers
        int64_t x[29];
        /// Frame pointer (x29)
        uint64_t fp;
        /// Link register (x30)
        uint64_t lr;
        /// Stack pointer (x31)
        uint64_t sp;
        /// Program Counter
        uint64_t pc;
        /// Flags (negative, zero, carry, overflow)
        uint16_t nzcv; 
    };
}