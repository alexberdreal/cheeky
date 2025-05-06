#pragma once
#include <stdint.h>

namespace cheeky::ops {
    // TODO: change to class
    struct State {
        /// General-purpose registers + fp (x29) + lr (x30) + sp / zr (x31)
        uint64_t r[32];
        /// Program Counter
        uint64_t pc;
        /// Flags (negative (bit 0), zero (bit 1), carry (bit 2), overflow (bit 3))
        std::bitset<4> nzcv; 

    // TODO: move to .cpp
    
        void set_n_flag(bool flag) {
            nzcv.set(0, flag);
        }

        bool get_n_flag() {
            return nzcv.test(0); 
        }

        void set_z_flag(bool flag) {
            nzcv.set(1, flag);
        }

        bool get_z_flag() {
            return nzcv.test(1); 
        }

        void set_c_flag(bool flag) {
            nzcv.set(2, flag);
        }

        bool get_c_flag() {
            return nzcv.test(2); 
        }

        void set_v_flag(bool flag) {
            nzcv.set(3, flag);
        }

        bool get_v_flag() {
            return nzcv.test(3); 
        }
    };
}