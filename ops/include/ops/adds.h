#pragma once
#include <ops/base_op.h>
#include <type_traits>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// sf   0   1   1   0   0   0   1   0   sh |            imm12                  |    Rn   |     Rd

/*
    32-bit (sf == 0)
    ADD <Wd|WSP>, <Wn|WSP>, #<imm>{, <shift>}
    64-bit (sf == 1)
    ADD <Xd|SP>, <Xn|SP>, #<imm>{, <shift>}
*/

// TODO: add support of S instructions (simply update NZCV flags if bit 29 is set)
// TODO: try partial specialization for S flag 

namespace cheeky::ops {
    class AddsImm : public BaseOperation {
    public:
        constexpr AddsImm() : BaseOperation(0b1100010, 1, 1) {}
        void process(uint32_t bits, State &state) override;
    };

    void AddsImm::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        auto is_sh_set = is_bit_set(bits, 22);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto imm12 = (bits & get_mask_from_bits(10, 21)) >> 10;

        auto update_nzcv = [&state](auto rn, auto imm) {
            static_assert(std::is_same_v<decltype(rn), uint32_t> || std::is_same_v<decltype(rn), uint64_t>);

            auto res = rn + imm;

            if (std::is_same_v<decltype(rn), uint32_t>) {
                // rn + imm > max -> rn > max - imm < rn
                state.set_c_flag(UINT32_MAX - imm < rn);

                // if the highest bit is 1 (negative sign)
                state.set_n_flag(res & (1 << 31));

                // Having y = x + C, if x and C share the same sign, but result has different sign, there is an overflow 
                // [1]0000..0 (- min) + [1]1111..1 (-1) = [0]111111..1 (+ max) -> overflow
                // [0]1111..1 (+ max) + [0]0000..1 (+1) = [1]000000..0 (- min) -> overflow
                // but, 
                // [1]1111..1 (-1) + [0]0000..1 (+1) = [0]0000..0 (+0) -> there is no overflow
                
                // check if Rn and Imm share the same sign
                auto is_same_sign = (rn & (1 << 31)) == (imm & (1 << 31));
                // check if sign's been changed
                auto v_flag = is_same_sign && ((res & (1 << 31)) != (rn & (1 << 31)));

                state.set_v_flag(v_flag);
            } else {
                // rn + imm > max -> rn > max - imm < rn
                state.set_c_flag(UINT64_MAX - imm < rn);

                // if the highest bit is 1 (negative sign)
                state.set_n_flag(res & (1 << 63));
                
                // Having y = x + C, if x and C share the same sign, but result has different sign, there is an overflow 
                // [1]0000..0 (- min) + [1]1111..1 (-1) = [0]111111..1 (+ max) -> overflow
                // [0]1111..1 (+ max) + [0]0000..1 (+1) = [1]000000..0 (- min) -> overflow
                // but, 
                // [1]1111..1 (-1) + [0]0000..1 (+1) = [0]0000..0 (+0) -> there is no overflow
                                
                // check if Rn and Imm share the same sign
                auto is_same_sign = (rn & (1 << 63)) == (imm & (1 << 63));
                // check if sign's been changed
                auto v_flag = is_same_sign && ((res & (1 << 63)) != (rn & (1 << 63)));
                
                state.set_v_flag(v_flag);
            }

            state.set_z_flag(rn + imm == 0);
        };

        auto add = [=](auto& rd, auto rn) { 
            auto imm = imm12; 
            if (is_sh_set) {
                imm <<= 12;
            }
            update_nzcv(rn, imm);
            rd = rn + imm;
        };

        if (is_sf_set) {
            // 64 bit case

            auto& rd = state.r[rd_idx];
            const auto& rn = state.r[rn_idx];

            add(rd, rn);
        } else {
            // 32 bit case

            auto rd = reinterpret_cast<uint32_t*>(&state.r[rd_idx]);
            auto rn = reinterpret_cast<const uint32_t*>(&state.r[rn_idx]);

            add(*rd, *rn);
        }
    }
}