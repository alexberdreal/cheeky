#include <ops/movz.h>

namespace cheeky::ops {
    void Movz::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto imm16 = (bits & get_mask_from_bits(5, 20)) >> 5;
        auto hw = (bits & get_mask_from_bits(21, 22)) >> 21;
        // TODO: validate for 32/64 bits version
        auto shft = hw * 16; 

        if (is_sf_set) {
            // 64 bit case

            auto& rd = state.get_r_ref(rd_idx);

            rd = (imm16 << shft);
        } else {
            // 32 bit case

            auto rd = reinterpret_cast<uint32_t*>(state.get_r_ptr(rd_idx));

            *rd = (imm16 << shft);
        }
    }
}