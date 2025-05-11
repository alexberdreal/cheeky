#pragma once
#include <ops/base_op.h>

/// Post-index

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8 7  6  5  4  3  2  1  0
// 1   x   1   1   1   0   0   0   0   1   0 |               imm9                |  0  1 |     Rn       |      Rt

/*
    32-bit (size == 10)
    LDR <Wt>, [<Xn|SP>], #<simm>
    64-bit (size == 11)
    LDR <Xt>, [<Xn|SP>], #<simm>
*/

/// Pre-index

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8 7  6  5  4  3  2  1  0
// 1   x   1   1   1   0   0   0   0   1   0 |               imm9                |  1  1 |     Rn       |      Rt

/*
    32-bit (size == 10)
    LDR <Wt>, [<Xn|SP>, #<simm>]!
    64-bit (size == 11)
    LDR <Xt>, [<Xn|SP>, #<simm>]!
*/

// Unsigned offset

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8 7  6  5  4  3  2  1  0
// 1   x   1   1   1   0   0   1   0   1  |                 imm12                         |     Rn      |      Rt

/*
    32-bit (size == 10)
    STR <Wt>, [<Xn|SP>{, #<pimm>}]
    64-bit (size == 11)
    STR <Xt>, [<Xn|SP>{, #<pimm>}]
*/

namespace cheeky::ops {
    class LdrImm : public BaseOperation {
        using State = core::State;
    private: 
        bool is_pre_post_idx(uint32_t bits);
        bool is_unsgn_offset(uint32_t bits);
    public:
        constexpr LdrImm() : BaseOperation(0b11100, 0, 2) {}
        void process(uint32_t bits, State &state) override;
        bool is_match(uint32_t bits) override;
    };
}