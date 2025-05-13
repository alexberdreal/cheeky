#include <ops/bl.h>

namespace cheeky::ops {
    bool Bl::process(uint32_t bits, State &state) {

        auto simm32 = (bits & get_mask_from_bits(0, 25)) << 6;

        auto simm32_shft = static_cast<int32_t>(shift_by_rule_32(ShiftRule::ASR, simm32, 6));

        // update link register to continue execution after branch is executed
        state.get_r_ref_64(30) = state.get_pc() + 1;
        
        // update PC
        state.update_pc(state.get_pc() + simm32_shft - 1);

        return true;
    }
}