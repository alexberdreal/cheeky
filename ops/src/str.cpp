#include <ops/str.h>

namespace cheeky::ops {
    bool StrImm::process(uint32_t bits, State &state) {
        auto rt_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto sz = (bits & get_mask_from_bits(30, 31)) >> 30; 
                
        auto str_pre_post_idx = [&state, bits = bits](auto& dst, auto& rn, auto rt, auto simm9) -> bool {
            auto enc = (bits & get_mask_from_bits(10, 11)) >> 11;
            
            if (enc == 0b11) {
                rn += simm9;
                dst = rt;
            } else if (enc == 0b01) {
                dst = rt;
                rn += simm9;
            } else {
                std::cerr << "Unknown StrImm instruction encoding, fatal error: " << std::hex << bits << std::endl;
                return false;
            }

            return true;
        };

        if (sz == 0b10) {   
            auto& rn = state.get_r_ref_32(rn_idx);
            auto rt = (rt_idx == 31) ? 0 : state.get_r_ref_32(rt_idx);

            if (is_unsgn_offset(bits)) {
                int32_t simm9 = ((bits & get_mask_from_bits(10, 21)) >> 10) * 4;
                auto& dst = state.get_vm_with_offset_32(rn + simm9);
                dst = rt;
            } else if (is_pre_post_idx(bits)) {
                int32_t simm9 = (bits & get_mask_from_bits(12, 20)) >> 12;
                auto& dst = state.get_vm_with_offset_32(rn + simm9);
                return str_pre_post_idx(dst, rn, rt, simm9);
            } else 
            {
                std::cerr << "Unknown StrImm instruction format, fatal error: " << std::hex << bits << std::endl;
                return false;
            }
        } else if (sz == 0b11) {
            auto& rn = state.get_r_ref_64(rn_idx);
            auto rt = (rt_idx == 31) ? 0 : state.get_r_ref_64(rt_idx);

            if (is_unsgn_offset(bits)) {
                int64_t simm9 = ((bits & get_mask_from_bits(10, 21)) >> 10) * 8;
                auto& dst = state.get_vm_with_offset_64(rn + simm9);
                dst = rt;
            } else if (is_pre_post_idx(bits)) {
                int64_t simm9 = (bits & get_mask_from_bits(12, 20)) >> 12;
                auto& dst = state.get_vm_with_offset_64(rn + simm9);
                return str_pre_post_idx(dst, rn, rt, simm9);
            } else 
            {
                std::cerr << "Unknown StrImm instruction format, fatal error: " << std::hex << bits << std::endl;
                return false;
            }
        } else {
            std::cerr << "fatal error: unknown size of StrImm op: " << sz << " bits: " << bits << std::endl;
            return false;
        }

        return true;
    }

    bool StrImm::is_pre_post_idx(uint32_t bits) {
        return ((bits & get_mask_from_bits(21, 29)) >> 21) == 0b111000000 && ((bits & get_mask_from_bits(10, 11)) >> 10) != 0;
    }

    bool StrImm::is_unsgn_offset(uint32_t bits) {
        return ((bits & get_mask_from_bits(22, 29)) >> 22) == 0b11100100;
    }

    bool StrImm::is_match(uint32_t bits) {
        return is_pre_post_idx(bits) || is_unsgn_offset(bits);
    }
}