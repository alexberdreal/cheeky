#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// sf   1   0   1   0   0   0   1   0   sh |            imm12                  |    Rn   |     Rd

/*
    32-bit (sf == 0)
    SUB <Wd|WSP>, <Wn|WSP>, #<imm>{, <shift>}
    64-bit (sf == 1)
    SUB <Xd|SP>, <Xn|SP>, #<imm>{, <shift>}
*/

// TODO: add support of S instructions (simply update NZCV flags if bit 29 is set)

namespace cheeky::ops {
    class SubImm : public BaseOperation {
        using State = core::State;
    public:
        constexpr SubImm() : BaseOperation(0b10100010, 0, 1) {}
        void process(uint32_t bits, State &state) override;
    };
}