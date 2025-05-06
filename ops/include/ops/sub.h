#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// sf   1   0   1   0   0   0   1   0   sh |            imm12                  |    Rn   |     Rd

/*
    32-bit (sf == 0)
    SUB <Wd|WSP>, <Wn|WSP>, #<imm>{, <shift>}
    64-bit (sf == 1)
    SUB <Xd|SP>, <Xn|SP>, #<imm>{, <shift>}
*/

// TODO: add support of S instructions (simply update NZCV flags if bit 29 is set)

namespace cheeky::ops {
    RegisterOperation(SubImm, 0b10100010, 0, 1);

    void SubImm::process(uint32_t bits, State &state) {
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

            auto& rd = state.r[rd_idx];
            const auto& rn = state.r[rn_idx];

            sub(rd, rn);
        } else {
            // 32 bit case

            auto rd = reinterpret_cast<uint32_t*>(&state.r[rd_idx]);
            auto rn = reinterpret_cast<const uint32_t*>(&state.r[rn_idx]);

            sub(*rd, *rn);
        }
    }
}