#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// 0    0   0   1   0   1  |                                 imm26         
/*
    B <label>

    <label> 
        Is the program label to be unconditionally branched to. Its offset from the address of this instruction, in
        the range +/-128MB, is encoded as "imm26" times 4.
*/

namespace cheeky::ops {
    class B : public BaseOperation {
        using State = core::State;
    public:
        constexpr B() : BaseOperation(0b101, 3, 0) {}
        bool process(uint32_t bits, State &state) override;
    };
}