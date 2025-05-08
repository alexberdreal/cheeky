#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// sf   0   0   1   0   0   0   1   0   sh |            imm12                  |    Rn   |     Rd

/*
    32-bit (sf == 0)
    ADD <Wd|WSP>, <Wn|WSP>, #<imm>{, <shift>}
    64-bit (sf == 1)
    ADD <Xd|SP>, <Xn|SP>, #<imm>{, <shift>}
*/

namespace cheeky::ops {
    class AddImm : public BaseOperation {
        using State = core::State;
    public:
        constexpr AddImm() : BaseOperation(0b100010, 2, 1) {}
        void process(uint32_t bits, State &state) override;
    };

    void AddImm::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        auto is_sh_set = is_bit_set(bits, 22);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto imm12 = (bits & get_mask_from_bits(10, 21)) >> 10;

        auto add = [=](auto& rd, auto rn) { 
            if (is_sh_set) {
                rd = rn + (imm12 << 12);
            } else {
                rd = rn + imm12; 
            }
        };

        if (is_sf_set) {
            // 64 bit case

            auto rd = state.get_r_ptr(rd_idx);
            auto rn = state.get_r_ptr(rn_idx);

            add(*rd, *rn);
        } else {
            // 32 bit case

            auto rd = reinterpret_cast<uint32_t*>(state.get_r_ptr(rd_idx));
            auto rn = reinterpret_cast<const uint32_t*>(state.get_r_ptr(rn_idx));

            add(*rd, *rn);
        }
    }
}