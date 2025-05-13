#include <ops/ldur.h>

namespace cheeky::ops {
    bool Ldur::process(uint32_t bits, State &state) {
        auto rt_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto sz = (bits & get_mask_from_bits(30, 31)) >> 30; 
        auto uimm9 = shift_by_rule_32(ShiftRule::ASR, (bits & get_mask_from_bits(12, 20)) << 11, 23); 

        if (sz == 0b10) {   // 32 bit
            auto rn = state.get_r_ref_32(rn_idx);
            auto& rt = state.get_r_ref_32(rt_idx);

            auto dst = state.get_vm_with_offset_32(rn + static_cast<int32_t>(uimm9));
            rt = dst;
        } else if (sz == 0b11) {    // 64 bit
            auto rn = state.get_r_ref_64(rn_idx);
            auto& rt = state.get_r_ref_64(rt_idx);

            auto dst = state.get_vm_with_offset_64(rn + static_cast<int32_t>(uimm9));
            rt = dst;
        } else {
            std::cerr << "fatal error: unknown size of Ldur op: " << sz << " bits: " << bits << std::endl;
            return false;
        }

        return true;
    }
}