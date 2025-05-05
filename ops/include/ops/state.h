#pragma once
#include <stdint.h>

namespace cheeky::ops {
    struct State {
        /// General-purpose registers + fp (x29) + lr (x30) + sp / zr (x31)
        uint64_t x[32];
        /// Program Counter
        uint64_t pc;
        /// Flags (negative, zero, carry, overflow)
        uint16_t nzcv; 
    };
}