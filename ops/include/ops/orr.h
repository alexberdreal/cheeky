#pragma once
#include <ops/base_op.h>

// 31   30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
// sf   0   1   0   1   0   1   0  |  sh  |  0 |        Rm         |        imm6          |      Rn      |     Rd

/*
    This instruction is used by the alias MOV (register).

    32-bit (sf == 0)
        ORR <Wd>, <Wn>, <Wm>{, <shift> #<amount>}
    64-bit (sf == 1)
        ORR <Xd>, <Xn>, <Xm>{, <shift> #<amount>}    
*/

namespace cheeky::ops {
    REGISTER_OPERATION(OrrSh)
}