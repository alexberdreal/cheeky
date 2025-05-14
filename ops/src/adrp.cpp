#include <ops/adrp.h>

namespace cheeky::ops {
    bool Adrp::process(uint32_t bits, State &state) {
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto immhi = (bits & get_mask_from_bits(5, 23)) >> 5;
        auto immlo = (bits & get_mask_from_bits(29, 30)) >> 29;

        auto imm = immlo || (immhi << 2); 

        auto alligned_pc = state.get_pc() - state.get_pc() % 1024;

        state.get_r_ref_64(rd_idx) = alligned_pc + static_cast<int32_t>(imm) * 1024;
    
        return true;
    }

    bool Adrp::is_match(uint32_t instr) {
        return (instr & 0x9F000000) == 0x90000000;
    }
}