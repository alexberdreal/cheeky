#pragma once
#include <core/logger.h>

#include <stdint.h>
#include <bitset>

const size_t STACK_SIZE = 512 * 1024;

namespace cheeky::core {
    class State {
    private:
        /// General-purpose registers + fp (x29) + lr (x30) + sp / zr (x31)
        uint64_t _r[32];
        /// Program Counter
        uint64_t _pc = 0;
        /// Flags (negative (bit 0), zero (bit 1), carry (bit 2), overflow (bit 3))
        std::bitset<4> _nzcv; 

        // TODO: malloc, now just 512 * 1024 bytes (512 KB)
        std::uint8_t _stack[STACK_SIZE];

        Logger _logger { "State" };
    
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
        void update_pc(uint64_t new_pc);

        // General-purpose registers operations
        
        uint32_t& get_r_ref_32(size_t idx);
        uint64_t& get_r_ref_64(size_t idx); 

        // Virtual memory accessors
        // Only stack is supported at the moment
        uint32_t& get_vm_with_offset_32(int32_t offset);
        uint64_t& get_vm_with_offset_64(int64_t offset);
    };
}