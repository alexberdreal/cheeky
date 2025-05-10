#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// sf   1   0   1   0   0   1   0   1  |  hw  |                     imm16              |     Rd

/*
    32-bit (sf == 0 && hw == 0x)
    MOVZ <Wd>, #<imm>{, LSL #<shift>}
    64-bit (sf == 1)
    MOVZ <Xd>, #<imm>{, LSL #<shift>}   
*/

namespace cheeky::ops {
    class Movz : public BaseOperation {
        using State = core::State;
    public:
        constexpr Movz() : BaseOperation(0b10100101, 0, 1) {}
        void process(uint32_t bits, State &state) override;
    };
}