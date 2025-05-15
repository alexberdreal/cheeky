#include <ops/tbnz.h>

// 1 0110111 11111 11110000011111 00100
//                 b40           rt

namespace cheeky::ops {
    bool Tbnz::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        
        auto rt_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto imm14 = (bits & get_mask_from_bits(5, 18)) >> 5;
        auto b40 = (bits & get_mask_from_bits(19, 23)) >> 19;
        auto b5 = (bits & get_mask_from_bits(31, 31)) >> 31;

        if (is_sf_set) {
            auto bit_pos = (b5 << 5) | b40;

            // 64 bit case
            if (state.get_r_ref_64(rt_idx) & (uint64_t(1) << bit_pos)) {
                state.update_pc(state.get_pc() + imm14 - 1);
            }
        } else {
            auto bit_pos = b40;
            
            if (state.get_r_ref_32(rt_idx) & (1 << bit_pos)) {
                state.update_pc(state.get_pc() + imm14 - 1);
            }
        }

        return true;
    }
}