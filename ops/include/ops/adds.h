#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// sf   0   1   1   0   0   0   1   0   sh |            imm12                  |    Rn   |     Rd

/*
    32-bit (sf == 0)
    ADD <Wd|WSP>, <Wn|WSP>, #<imm>{, <shift>}
    64-bit (sf == 1)
    ADD <Xd|SP>, <Xn|SP>, #<imm>{, <shift>}
*/

namespace cheeky::ops {
    class AddsImm : public BaseOperation {
        using State = core::State;
    public:
        constexpr AddsImm() : BaseOperation(0b1100010, 1, 1) {}
        bool process(uint32_t bits, State &state) override;
    };
}