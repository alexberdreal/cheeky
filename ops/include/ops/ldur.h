#pragma once
#include <ops/base_op.h>

/// Post-index

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0
// 1   x   1   1   1   0   0   0   0   1   0  |               imm9               | 0   0 |     Rn       |      Rt

/*
    32-bit (size == 10)
    LDUR <Wt>, [<Xn|SP>{, #<simm>}]
    64-bit (size == 11)
    LDUR <Xt>, [<Xn|SP>{, #<simm>}]
*/

namespace cheeky::ops {
    REGISTER_OPERATION(Ldur)
}