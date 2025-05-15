#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
// b5   0   1   1   0   1   1   1  |      b40          |                        imm14                     |      Rt

/*
    TBNZ <R><t>, #<imm>, <label>
*/

namespace cheeky::ops {
    class Tbnz : public BaseOperation {
        using State = core::State;
    public:
        constexpr Tbnz() : BaseOperation(0b110111, 1, 1) {}
        bool process(uint32_t bits, State &state) override;
    };
}