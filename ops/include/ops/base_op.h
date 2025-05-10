#pragma once
#include <iostream>
#include <assert.h>
#include <utility>

#include <core/state.h>
#include <core/util.h>

namespace cheeky::ops {
    // TODO: move to ops utils

    enum class OpName : uint8_t {
        AddImm, SubImm, AddsImm, SubsImm, OrrSh, Movz // TODO: Str, Ldr, Ret
    };

    enum class ShiftRule : uint8_t {
        LSL = 0, LSR = 0b01, ASR = 0b10, ROR = 0b11, // TODO check if invalid
    };

    // 0b010001 (X lower bits) -> 0b01000100000000000000000000000000 (32 bits with shifted to higher)
    constexpr uint32_t adjust_bits(uint32_t bits, uint32_t zeros) {
        auto bits_len = core::log2(bits) + 1;
        return bits << (31u - bits_len - zeros);
    }

    // 0b01[000110]0010110.. -> 0b000110 (6 bits)
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
        auto mask_len = core::log2(raw_fixed) + 1 + zeros;
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

    // TODO: tests + validate rule
    inline uint64_t shift_by_rule_64(ShiftRule rule, uint64_t data, uint16_t amt) {
        assert((0 <= amt) &&  (amt <= 63));

        if (amt == 0) {
            return data;
        }

        switch (rule)
        {
        case ShiftRule::LSL:
            return data << amt;                
        case ShiftRule::LSR:
            return data >> amt;
        case ShiftRule::ASR: {
            auto is_neg = data & (uint64_t(1) << 63);
            if (is_neg) {
                // 1. Reset the sign bit
                data &= ((uint64_t(1) << 63) - 1);
                // 2. Do a LSR by <amt>
                data >>= amt;
                // 3. Get sequential 11..1 bits with sign bit + shifted bits.
                data |= (((uint64_t(1) << (amt + 1)) - 1) << (63 - amt));

                return data;
            } else {
                // For positive - behave like a regular LSR
                return data >>= amt;
            }
        }
        case ShiftRule::ROR: {
            // Rotate right, 0b001100[10] >> 2 => 0b[10]1100

            // 1. Get last <amt> bits, shift them left by 64 - <amt> bits
            auto mirrored = (((uint64_t(1) << amt) - 1) & data) << (64 - amt); 
            // 2. Do a regular LSR 
            data >>= amt;
            // 3. Do OR with the value from step 1
            data |= mirrored;

            return data;
        }
        default: {
            // TODO
            std::cerr << "Shft rule default" << std::endl;
            std::terminate();
        }
        }
    } 

       // TODO: tests + validate rule
       constexpr uint32_t shift_by_rule_32(ShiftRule rule, uint32_t data, uint16_t amt) {
        assert((0 <= amt) &&  (amt <= 31));

        if (amt == 0) {
            return data;
        }

        switch (rule)
        {
        case ShiftRule::LSL:
            return data << amt;
        case ShiftRule::LSR:
            return data >>= amt;
        case ShiftRule::ASR: {
            auto is_neg = data & (1u << 31);
            if (is_neg) {
                // 1. Reset the sign bit
                data &= ((1u << 31) - 1);
                // 2. Do a LSR by <amt>
                data >>= amt;
                // 3. Get sequential 11..1 bits with sign bit + shifted bits.
                data |= (((1 << (amt + 1)) - 1) << (31 - amt));

                return data;
            } else {
                // For positive - behave like a regular LSR
                return data >>= amt;
            }
        }
        case ShiftRule::ROR: {
            // Rotate right, 0b001100[10] >> 2 => 0b[10]1100

            // 1. Get last <amt> bits, shift them left by 64 - <amt> bits
            auto mirrored = (((1 << amt) - 1) & data) << (32 - amt); 
            // 2. Do a regular LSR 
            data >>= amt;
            // 3. Do OR with the value from step 1
            data |= mirrored;

            return data;
        }
        default: {
            // TODO
            std::cerr << "Shft rule default" << std::endl;
            std::terminate();
            break;
        }
        }
    } 

    // sz - skipped zeros
    // iz - insignigicant zeros
    class BaseOperation {
        using State = core::State;
    private: 
        uint32_t _fixed_bits;          
        uint32_t _mask;    
        uint32_t _base_fixed_bits; 
    public: 
        /// raw_fixed - fixed bits, starting from 1
        /// iz (insignificant zeros) - number of zeros, at the start of fixed bits
        /// sb (skipped bits) - number of skipped bits at the start of instruction
        /// 
        /// Example: 
        /// bytes:              31 30 29 28 27 26 25 24 23 22 21 ...
        /// instruction format: x  x  0  1  1  0  1  0  0  0  1  ... 
        /// sb = 2 (number of x's), sb = 1 (only bit 29), raw_fixed = 0x11010001
        constexpr BaseOperation(uint32_t raw_fixed, uint32_t iz, uint32_t sb) :
            _fixed_bits(adjust_bits(raw_fixed, iz)), 
            _base_fixed_bits(get_base_fixed_bits(_fixed_bits)), 
            _mask(get_mask_from_fixed(raw_fixed, iz, sb)) {
                // base mask should start at least from a third higher bit
                assert(sb <= 2);
            }

        constexpr uint32_t base_fixed_bits() {
            return _base_fixed_bits;
        }

        virtual bool is_match(uint32_t bits) {
            return (bits & _mask) == _fixed_bits;
        }

        virtual void process(uint32_t data, State&) = 0;
    };
}