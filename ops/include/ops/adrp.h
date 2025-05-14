#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
// 1  | immlo |  1  0   0   0   0 |                                    immhi                           |      Rd
/*
    ADRP <Xd>, <label>

    <label> 
        Is the program label whose 4KB page address is to be calculated. Its offset from the page address of
        this instruction, in the range +/-4GB, is encoded as "immhi:immlo" times 4096.
*/

namespace cheeky::ops {
    class Adrp : public BaseOperation {
        using State = core::State;
    public:
        constexpr Adrp() : BaseOperation(0b10000, 0, 3) {}
        bool process(uint32_t bits, State &state) override;
        bool is_match(uint32_t instr) override; 
    };
}