#include <ops/csinc.h>
#include <core/state.h>

enum class CondCode : uint8_t {
    EQ = 0x0,  // Equal (Z == 1)
    NE = 0x1,  // Not equal (Z == 0)
    CS = 0x2,  // Carry set / Unsigned >= (C == 1)
    CC = 0x3,  // Carry clear / Unsigned < (C == 0)
    MI = 0x4,  // Minus / Negative (N == 1)
    PL = 0x5,  // Plus / Positive or zero (N == 0)
    VS = 0x6,  // Overflow (V == 1)
    VC = 0x7,  // No overflow (V == 0)
    HI = 0x8,  // Unsigned higher (C == 1 && Z == 0)
    LS = 0x9,  // Unsigned lower or same (C == 0 || Z == 1)
    GE = 0xA,  // Signed greater or equal (N == V)
    LT = 0xB,  // Signed less than (N != V)
    GT = 0xC,  // Signed greater than (Z == 0 && N == V)
    LE = 0xD,  // Signed less or equal (Z == 1 || N != V)
    AL = 0xE,  // Always (unconditional)
    NV = 0xF   // Never (reserved / unpredictable)
};

bool is_cond_true(const cheeky::core::State& state, uint8_t cond) {
    switch (static_cast<CondCode>(cond))
    {
    case CondCode::EQ: 
        return state.get_z_flag();
    case CondCode::NE:
        return !state.get_z_flag();
    case CondCode::CS:
        return state.get_c_flag();
    case CondCode::CC:
        return !state.get_c_flag();
    case CondCode::MI:
        return state.get_n_flag();
    case CondCode::PL:
        return !state.get_n_flag();
    case CondCode::VS:
        return state.get_v_flag();
    case CondCode::VC:
        return !state.get_v_flag();
    case CondCode::HI:
        return state.get_c_flag() && !state.get_z_flag();
    case CondCode::LS: 
        return !state.get_c_flag() && state.get_z_flag();
    case CondCode::GE:
        return state.get_n_flag() == state.get_v_flag();
    case CondCode::LT:
        return state.get_n_flag() != state.get_v_flag();
    case CondCode::GT:
        return !state.get_z_flag() && state.get_n_flag() == state.get_v_flag();
    case CondCode::LE:
        return state.get_z_flag() && state.get_n_flag() != state.get_v_flag();
    case CondCode::AL:
        return true;
    case CondCode::NV:
        return false;
    default:    // should be validated outside 
        break;
    }
}

namespace cheeky::ops {
    bool Csinc::process(uint32_t bits, State &state) {
        auto is_sf_set = is_bit_set(bits, 31);
        
        auto rd_idx = (bits & get_mask_from_bits(0, 4)) >> 0;
        auto rn_idx = (bits & get_mask_from_bits(5, 9)) >> 5;
        auto cond = (bits & get_mask_from_bits(12, 15)) >> 12;
        auto rm_idx = (bits & get_mask_from_bits(16, 20)) >> 16;

        auto is_cset = rm_idx == 0b11111 && ((cond >> 1) != 0b111) && rn_idx == 0b11111;

        assert(0 <= cond && cond <= 0xF);

        if (is_sf_set) {
            if (is_cset) {
                cond ^= 1;
                state.get_r_ref_64(rd_idx) = is_cond_true(state, cond);
                return true;
            }
            // 64 bit case
            if (is_cond_true(state, cond)) {
                state.get_r_ref_64(rd_idx) = (rn_idx == 31) ? 0 : state.get_r_ref_64(rn_idx);
            } else {
                state.get_r_ref_64(rd_idx) = ((rm_idx == 31) ? 0 : state.get_r_ref_64(rm_idx)) + 1;
            }
        } else {
            if (is_cset) {
                cond ^= 1;
                state.get_r_ref_32(rd_idx) = is_cond_true(state, cond);
                return true;
            }

            if (is_cond_true(state, cond)) {
                state.get_r_ref_32(rd_idx) = (rn_idx == 31) ? 0 : state.get_r_ref_32(rn_idx);
            } else {
                state.get_r_ref_32(rd_idx) = ((rm_idx == 31) ? 0 : state.get_r_ref_32(rm_idx)) + 1;
            }
        }

        return true;
    }
}