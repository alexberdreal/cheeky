#pragma once
#include <ops/base_op.h>

/// Post-index

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
// x   0   1   0   1   0   0   0   1   0  |         imm7             |       Rt2         |      Rn      |      Rt

/*
    32-bit (opc == 00)
    STP <Wt1>, <Wt2>, [<Xn|SP>{, #<imm>}]
    64-bit (opc == 10)
    STP <Xt1>, <Xt2>, [<Xn|SP>{, #<imm>}]
*/

/// Pre-index

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8 7  6  5  4  3  2  1  0
// x   0   1   0   1   0   0   1   1   0  |         imm7             |       Rt2         |      Rn      |      Rt

/*
    32-bit (opc == 00)
    STP <Wt1>, <Wt2>, [<Xn|SP>, #<imm>]!
    64-bit (opc == 10)
    STP <Xt1>, <Xt2>, [<Xn|SP>, #<imm>]!
*/

// Signed offset

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8 7  6  5  4  3  2  1  0
// x   0   1   0   1   0   0   1   0   0  |                 imm12                         |     Rn      |      Rt

/*
    32-bit (opc == 00)
    STP <Wt1>, <Wt2>, [<Xn|SP>{, #<imm>}]
    64-bit (opc == 10)
    STP <Xt1>, <Xt2>, [<Xn|SP>{, #<imm>}]
*/

namespace cheeky::ops {
    class Stp : public BaseOperation {
        using State = core::State;
    private: 
        bool is_pre_post_idx(uint32_t bits);
        bool is_sgn_offset(uint32_t bits);
    public:
        constexpr Stp() : BaseOperation(0b10100, 1, 1) {}
        bool process(uint32_t bits, State &state) override;
        bool is_match(uint32_t bits) override;
    };
}