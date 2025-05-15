#include <ops/stur.h>

namespace cheeky::ops {
    bool Stur::process(uint32_t bits, State &state) {
        auto rt_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto sz = (bits & get_mask_from_bits(30, 31)) >> 30; 
        auto uimm9 = shift_by_rule_32(ShiftRule::ASR, (bits & get_mask_from_bits(12, 20)) << 11, 23); 

        if (sz == 0b10) {   
            auto rn = state.get_r_ref_32(rn_idx);
            auto rt = (rt_idx == 31) ? 0 : state.get_r_ref_32(rt_idx);

            auto& dst = state.get_vm_with_offset_32(rn + static_cast<int32_t>(uimm9));
            dst = rt;
        } else if (sz == 0b11) {
            auto rn = state.get_r_ref_64(rn_idx);

            auto rt = (rt_idx == 31) ? 0 : state.get_r_ref_64(rt_idx);

            auto& dst = state.get_vm_with_offset_64(rn + static_cast<int32_t>(uimm9));
            dst = rt;
        } else {
            std::cerr << "fatal error: unknown size of StrImm op: " << sz << " bits: " << bits << std::endl;
            return false;
        }

        return true;
    }
}