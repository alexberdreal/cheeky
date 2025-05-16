#pragma once
#include <ops/base_op.h>

// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10  9   8  7  6  5  4  3  2  1  0
// 1   1   0   1   0   1   1   0   0   1   0   1   1   1   1   1   0   0   0   0   0   0  |      Rn      | 0  0  0  0  0

/*
    RET {<Xn>}

    <Xn> Is the 64-bit name of the general-purpose register holding the address to be branched to, encoded in
    the "Rn" field. Defaults to X30 if absent.
*/

namespace cheeky::ops {
    REGISTER_OPERATION(Ret)
}