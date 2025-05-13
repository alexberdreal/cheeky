#include <ops/ldr.h>

namespace cheeky::ops {
    bool LdrImm::process(uint32_t bits, State &state) {
        auto rt_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto sz = (bits & get_mask_from_bits(30, 31)) >> 30; 
                
        auto str_pre_post_idx = [&state, bits = bits](auto src, auto& rn, auto& rt, auto simm9) -> bool {
            auto enc = (bits & get_mask_from_bits(10, 11)) >> 11;
            
            if (enc == 0b11) {
                rt = src;
                rn += simm9;
            } else if (enc == 0b01) {
                rn += simm9;
                rt = src;
            } else {
                std::cerr << "Unknown LdrImm instruction encoding, fatal error: " << std::hex << bits << std::endl;
                return false;
            }

            return true;
        };

        if (sz == 0b10) {   
            auto& rn = state.get_r_ref_32(rn_idx);
            auto& rt = state.get_r_ref_32(rt_idx);

            if (is_unsgn_offset(bits)) {
                int32_t simm9 = ((bits & get_mask_from_bits(10, 21)) >> 10) * 4;
                auto src = state.get_vm_with_offset_32(rn + simm9);
                rt = src;
            } else if (is_pre_post_idx(bits)) {
                int32_t simm9 = (bits & get_mask_from_bits(12, 20)) >> 12;
                auto src = state.get_vm_with_offset_32(rn + simm9);
                return str_pre_post_idx(src, rn, rt, simm9);
            } else 
            {
                std::cerr << "Unknown LdrImm instruction format, fatal error: "  << std::hex << bits << std::endl;
                return false;
            }
        } else if (sz == 0b11) {
            auto& rn = state.get_r_ref_64(rn_idx);
            auto rt = state.get_r_ref_64(rt_idx);

            if (is_unsgn_offset(bits)) {
                int64_t simm9 = ((bits & get_mask_from_bits(10, 21)) >> 10) * 8;
                auto src = state.get_vm_with_offset_64(rn + simm9);
                rt = src;
            } else if (is_pre_post_idx(bits)) {
                int64_t simm9 = (bits & get_mask_from_bits(12, 20)) >> 12;
                auto src = state.get_vm_with_offset_64(rn + simm9);
                return str_pre_post_idx(src, rn, rt, simm9);
            } else 
            {
                std::cerr << "Unknown LdrImm instruction format, fatal error: " << std::hex << bits << std::endl;
                return false;
            }
        } else {
            std::cerr << "fatal error: unknown size of LdrImm op: " << sz << " bits: " << bits << std::endl;
            return false;
        }

        return true;
    }

    bool LdrImm::is_pre_post_idx(uint32_t bits) {
        return ((bits & get_mask_from_bits(21, 29)) >> 21) == 0b111000010 && (bits & get_mask_from_bits(10, 11)) >> 10 != 0;
    }

    bool LdrImm::is_unsgn_offset(uint32_t bits) {
        return ((bits & get_mask_from_bits(22, 29)) >> 22) == 0b11100101;
    }

    bool LdrImm::is_match(uint32_t bits) {
        return is_pre_post_idx(bits) || is_unsgn_offset(bits);
    }
}