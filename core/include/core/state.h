#pragma once
#include <stdint.h>
#include <bitset>

namespace cheeky::core {
    class State {
    private:
        /// General-purpose registers + fp (x29) + lr (x30) + sp / zr (x31)
        uint64_t _r[32];
        /// Program Counter
        uint64_t _pc;
        /// Flags (negative (bit 0), zero (bit 1), carry (bit 2), overflow (bit 3))
        std::bitset<4> _nzcv; 
    
    public:
        // NZCV flags 

        void set_n_flag(bool flag);
        bool get_n_flag() const;

        void set_z_flag(bool flag);
        bool get_z_flag() const;

        void set_c_flag(bool flag);
        bool get_c_flag() const;

        void set_v_flag(bool flag);
        bool get_v_flag() const;

        // PC operations

        void advance_pc();
        uint64_t get_pc() const;

        // General-purpose registers operations
        
        uint64_t* get_r_ptr(size_t idx);
        uint64_t& get_r_ref(size_t idx); 
    };
}