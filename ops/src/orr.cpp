#include <ops/orr.h>

namespace cheeky::ops {
    void OrrSh::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto imm6 = (bits & get_mask_from_bits(10, 15)) >> 10;
        auto rm_idx = (bits & get_mask_from_bits(16, 20)) >> 16;
        auto sh_bits = (bits & get_mask_from_bits(22, 23)) >> 22;
        auto sh_rule = static_cast<ShiftRule>(sh_bits);

        if (is_sf_set) {
            // 64 bit case

            auto& rd = state.get_r_ref(rd_idx);
            auto rm = state.get_r_ref(rm_idx);
            auto rm_shft = shift_by_rule_64(sh_rule, rm, imm6);
            if (rn_idx != 31) {
                auto rn = state.get_r_ref(rn_idx);
                rd = (rn | rm_shft);
            } else {
                rd = rm_shft;
                state.get_r_ref(rd_idx);
            }
        } else {
            // 32 bit case

            auto rd = reinterpret_cast<uint32_t*>(state.get_r_ptr(rd_idx));
            auto rm = reinterpret_cast<const uint32_t*>(state.get_r_ptr(rm_idx));\
            auto rm_shft = shift_by_rule_32(sh_rule, *rm, imm6);
            if (rn_idx != 31) {
                auto rn = reinterpret_cast<const uint32_t*>(state.get_r_ptr(rn_idx));\
                *rd = (*rn | rm_shft);
            } else {
                *rd = rm_shft;
            }
        }
    }
}