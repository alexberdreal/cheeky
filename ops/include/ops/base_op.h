#pragma once
#include <iostream>
#include <ops/state.h>
#include <assert.h>

#include <utils/util.h>

namespace cheeky::ops {
    enum class OpName {
        AddImm, AddShExt, Sub, Orr, SubImm, SubShExt
    };

    // 0b010001 (X lower bits) -> 0b01000100000000000000000000000000 (32 bits with shifted to higher)
    constexpr uint32_t adjust_bits(uint32_t bits, uint32_t zeros) {
        auto bits_len = utils::log2(bits) + 1;
        return bits << (31u - bits_len - zeros);
    }

    // 0b01[000110]0010110.. -> 0b000110
    constexpr uint32_t get_base_fixed_bits(uint32_t fixed) {
        return (fixed & 0x3F000000) >> 29;
    }

    // 0b01[00011000101]10.. -> 0b00[11111111111]00....
    // 0b01[00011000101]10
    // 0b11[11111111111]11..1
    // MAX >> skipped = 0b00111..
    // res >> 31 - skipped - len = 0b00[11111111111]
    // res << 31 - skipped - len
    constexpr uint32_t get_mask_from_fixed(uint32_t raw_fixed, uint32_t zeros, uint32_t skipped_bits) {
        auto mask_len = utils::log2(raw_fixed) + 1 + zeros;
        return ((1 << mask_len) - 1) << (32 - skipped_bits - mask_len);
    }

    constexpr bool is_bit_set(uint32_t bits, uint8_t num) {
        return (bits & (1 << num)) != 0;
    }

    constexpr uint32_t get_mask_from_bits(uint8_t start_bit, uint8_t end_bit) {
        assert(end_bit >= start_bit);
        auto len = end_bit - start_bit + 1;
        return ((1 << len) - 1) << start_bit;
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


    /// Registers a operation 
    /// name - name of an operation class, must be present in OpName enum
    /// fixed_raw - fixed bits, starting from 1
    /// iz (insignificant zeros) - number of zeros, at the start of fixed bits
    /// sb (skipped bits) - number of skipped bits at the start of instruction
    /// 
    /// Example: 
    // bytes:              31 30 29 28 27 26 25 24 23 22 21 ...
    // instruction format: x  x  0  1  1  0  1  0  0  0  1  ... 
    // sb = 2 (number of x's), sb = 1 (only bit 29), fixed_raw = 0x11010001
    #define RegisterOperation(name, fixed_raw, iz, sb)                               \
    template <>                                                                      \
    class BaseOperation<OpName::name, fixed_raw, iz, sb> {                           \
    public:                                                                          \
        static constexpr uint32_t fixed_bits = adjust_bits(fixed_raw, iz);           \
        static constexpr uint32_t base_fixed_bits = get_base_fixed_bits(fixed_bits); \
        static constexpr uint32_t mask = get_mask_from_fixed(fixed_raw, iz, sb);     \
        bool is_match(uint32_t bits) { return (bits & mask) == fixed_bits; }         \
        void process(uint32_t bits, State& state);                                   \
    };                                                                               \
    using name = BaseOperation<OpName::name, fixed_raw, iz, sb>;
}