#include <ops/ret.h>

namespace cheeky::ops {
    void Ret::process(uint32_t bits, State &state) {
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto& rn = state.get_r_ref_64(rn_idx);
        if (rn == 0) {
            state.deactivate();
        } else {
            // pass rn - 1, instead of just rn, because PC is incremented after each instruction execution
            state.update_pc(rn - 1);
            rn = 0;
        }
    }
}