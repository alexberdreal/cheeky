#include <ops/subs.h>

namespace cheeky::ops {
    void SubsImm::process(uint32_t bits, State &state) {

        auto is_sf_set = is_bit_set(bits, 31);
        auto is_sh_set = is_bit_set(bits, 22);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto imm12 = (bits & get_mask_from_bits(10, 21)) >> 10;

        auto update_nzcv = [&state](auto rn, auto imm) {
            static_assert(std::is_same_v<decltype(rn), uint32_t> || std::is_same_v<decltype(rn), uint64_t>);

            auto res = rn - imm;

            if (std::is_same_v<decltype(rn), uint32_t>) {
                // if the highest bit is 1 (negative sign)
                state.set_n_flag(res & (1u << 31));

                // Having y = x - C, if x and C have different signs, but result has a sign different than x's one, there is an overflow 
                // [1]0000..0 (- min) - [0]0000..1 (+1) = [0]111111..1 (+ max) -> overflow
                // [0]1111..1 (+ max) - [1]1111..1 (-1) = [1]000000..0 (- min) -> overflow
                // but, 
                // [1]1111..1 (-1) + [0]0000..1 (+1) = [0]0000..0 (+0) -> there is no overflow
                
                // check if Rn and Imm share the same sign
                auto is_same_sign = (rn & (1u << 31)) == (imm & (1u << 31));
                // check if sign's been changed
                auto v_flag = !is_same_sign && ((res & (1u << 31)) != (rn & (1u << 31)));

                state.set_v_flag(v_flag);
            } else {
                // if the highest bit is 1 (negative sign)
                state.set_n_flag(res & (uint64_t(1) << 63));
                
                // Having y = x + C, if x and C share the same sign, but result has different sign, there is an overflow 
                // [1]0000..0 (- min) + [1]1111..1 (-1) = [0]111111..1 (+ max) -> overflow
                // [0]1111..1 (+ max) + [0]0000..1 (+1) = [1]000000..0 (- min) -> overflow
                // but, 
                // [1]1111..1 (-1) + [0]0000..1 (+1) = [0]0000..0 (+0) -> there is no overflow
                                
                // check if Rn and Imm share the same sign
                auto is_same_sign = (rn & (uint64_t(1) << 63)) == (imm & (uint64_t(1) << 63));
                // check if sign's been changed
                auto v_flag = !is_same_sign && ((res & (uint64_t(1) << 63)) != (rn & (uint64_t(1) << 63)));
                
                state.set_v_flag(v_flag);
            }

            state.set_z_flag(rn + imm == 0);
            // subs sets the Carry flag in case of no underflow (no borrowing)
            // rn - imm >= min -> rn >= min + imm -> rn >= 0 + imm
            state.set_c_flag(imm <= rn);
        };

        auto sub = [=](auto& rd, auto rn) { 
            auto imm = imm12;
            if (is_sh_set) {
                imm <<= 12;
            } 
            rd = rn - imm12; 
            update_nzcv(rn, imm);
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
    }
}