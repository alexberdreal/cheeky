#pragma once
#include <iostream>
#include <state.h>

#include <utils/util.h>

namespace cheeky::ops {
    enum class OpName {
        add_imm, add_shft_ext, sub, orr, sub_imm, sub_shft
    };

    // 0b010001 (X lower bits) -> 0b01000100000000000000000000000000 (32 bits with shifted to higher)
    constexpr uint32_t adjust_bits(uint32_t bits, uint32_t zeros) {
        auto bits_len = utils::log2(bits);
        return bits << (31u - bits_len - zeros);
    }

    // 0b01[000110]0010110.. -> 0b000110
    constexpr uint32_t get_base_fixed_bits(uint32_t fixed) {
        return (fixed & 0x3F000000) >> 29;
    }

    // 0b01[00011000101]10.. -> 0b00[11111111111]00....
    constexpr uint32_t get_mask_from_fixed(uint32_t raw_fixed, uint32_t skipped_bits) {
        auto raw_fixed_len = utils::log2(raw_fixed);
        return (UINT32_MAX << 31 - raw_fixed_len - skipped_bits) & (UINT32_MAX >> 2);
    }

    // sz - skipped zeros
    // iz - insignigicant zeros
    template <OpName name, int32_t fixed_bits, int32_t sz, int32_t iz>
    class BaseOperation {

        bool is_match(uint32_t data) { 
            std::cerr << "instruction for " << data << " is undefined\n";
            return false;
        }

        void process(uint32_t data, State&) {
            std::cerr << "instruction for " << data << " is undefined\n";
        }
    };