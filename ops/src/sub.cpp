#include <ops/sub.h>

namespace cheeky::ops {
    bool SubImm::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        auto is_sh_set = is_bit_set(bits, 22);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto imm12 = (bits & get_mask_from_bits(10, 21)) >> 10;

        auto sub = [=](auto& rd, auto rn) { 
            if (is_sh_set) {
                rd = rn - (imm12 << 12);
            } else {
                rd = rn - imm12; 
            }
        };

        if (is_sf_set) {
            // 64 bit case

            auto& rd = state.get_r_ref_64(rd_idx);
            auto rn = state.get_r_ref_64(rn_idx);

            sub(rd, rn);
        } else {
            // 32 bit case

            auto& rd = state.get_r_ref_32(rd_idx);
            auto rn = state.get_r_ref_32(rn_idx);

            sub(rd, rn);
        }

        return true;
    }
}