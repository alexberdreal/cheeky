#include "assert.h"

#include <ops/add.h>
#include <ops/sub.h>

#include <ops/state.h>

using namespace cheeky::ops;

int main() {
    AddImm add_imm;
    SubImm sub_imm;
    
    State state;

    state.x[0] = 0;
    state.x[8] = 0;

    int32_t bits = 0x11400900;

    assert(add_imm.is_match(bits));

    add_imm.process(bits, state);

    assert(state.x[0] == 8192);

    state.x[8] = 500;
    
    {
        int32_t sub_bits = 0x5100C908;

        assert(sub_imm.is_match(sub_bits));
    
        sub_imm.process(sub_bits, state);
        
        assert(state.x[8] == 450);
    }

    {
        int32_t sub_bits = 0x5100C908;

        assert(sub_imm.is_match(sub_bits));
    
        sub_imm.process(sub_bits, state);
        
        assert(state.x[8] == 400);
    }

    return 0;
}