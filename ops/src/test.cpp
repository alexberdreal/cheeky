#include "assert.h"

#include <ops/add.h>
#include <ops/state.h>

using namespace cheeky::ops;

int main() {
    AddImm add_imm;
    State state;

    state.x[0] = 0;
    state.x[8] = 0;

    int32_t bits = 0x11400900;

    assert(add_imm.is_match(bits));

    add_imm.process(bits, state);

    assert(state.x[0] == 8192);
    
    return 0;
}