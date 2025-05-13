#include <ops/stp.h>

namespace cheeky::ops {
    bool Stp::process(uint32_t bits, State &state) {
        auto rt1_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto rt2_idx = (bits & get_mask_from_bits(10, 14)) >> 10;
        auto sz = (bits & get_mask_from_bits(30, 31)) >> 30; 
                
        auto str_pre_post_idx = [&state, bits = bits](auto& dst1, auto& dst2, auto& rn, auto rt1, auto rt2, auto simm9) -> bool {
            auto enc = (bits & get_mask_from_bits(23, 24)) >> 23;
            
            if (enc == 0b11) { // pre indexing 
                rn += simm9;
                dst1 = rt1;
                dst2 = rt2;
            } else if (enc == 0b01) { // post indexing
                dst1 = rt1;
                dst2 = rt2;
                rn += simm9;
            } else {
                std::cerr << "Unknown Stp instruction encoding, fatal error: " << std::hex << bits << std::endl;
                return false;
            }

            return true;
        };

        if (sz == 0b00) {   // 32-bit version
            auto& rn = state.get_r_ref_32(rn_idx);
            auto rt1 = (rt1_idx == 31) ? 0 : state.get_r_ref_32(rt1_idx);
            auto rt2 = (rt2_idx == 31) ? 0 : state.get_r_ref_32(rt2_idx);

            auto simm = ((bits & get_mask_from_bits(15, 21)) >> 15) * 4;
            auto& dst1 = state.get_vm_with_offset_32(rn + simm);
            auto& dst2 = state.get_vm_with_offset_32(rn + simm + 4);

            if (is_sgn_offset(bits)) {
                dst1 = rt1;
                dst2 = rt2;
            } else if (is_pre_post_idx(bits)) {
                return str_pre_post_idx(dst1, dst2, rn, rt1, rt2, simm);
            } else 
            {
                std::cerr << "Unknown Stp instruction format, fatal error: " << std::hex << bits << std::endl;
                return false;
            }
        } else if (sz == 0b10) {    // 64-bit version
            auto& rn = state.get_r_ref_64(rn_idx);
            auto rt1 = (rt1_idx == 31) ? 0 : state.get_r_ref_64(rt1_idx);
            auto rt2 = (rt2_idx == 31) ? 0 : state.get_r_ref_64(rt2_idx);

            auto simm = ((bits & get_mask_from_bits(15, 21)) >> 15) * 8;
            auto& dst1 = state.get_vm_with_offset_64(rn + simm);
            auto& dst2 = state.get_vm_with_offset_64(rn + simm + 8);

            if (is_sgn_offset(bits)) {
                dst1 = rt1;
                dst2 = rt2;
            } else if (is_pre_post_idx(bits)) {
                return str_pre_post_idx(dst1, dst2, rn, rt1, rt2, simm);
            } else 
            {
                std::cerr << "Unknown Stp instruction format, fatal error: " << std::hex << bits << std::endl;
                return false;
            }
        } else {
            std::cerr << "fatal error: unknown size of Stp op: " << sz << " bits: " << bits << std::endl;
            return false;
        }

        return true;
    }

    bool Stp::is_pre_post_idx(uint32_t bits) {
        auto fixed = ((bits & get_mask_from_bits(22, 30)) >> 22);
        return fixed == 0b010100010 || fixed == 0b010100110;
    }

    bool Stp::is_sgn_offset(uint32_t bits) {
        auto fixed = ((bits & get_mask_from_bits(22, 30)) >> 22);
        return fixed == 0b010100100;
    }

    bool Stp::is_match(uint32_t bits) {
        return is_pre_post_idx(bits) || is_sgn_offset(bits);
    }
}