#include <ops/b.h>

namespace cheeky::ops {
    bool B::process(uint32_t bits, State &state) {

        auto simm32 = (bits & get_mask_from_bits(0, 25)) << 6;

        auto simm32_shft = static_cast<int32_t>(shift_by_rule_32(ShiftRule::ASR, simm32, 6));
        
        // update PC
        state.update_pc(state.get_pc() + simm32_shft - 1);

        return true;
    }
}