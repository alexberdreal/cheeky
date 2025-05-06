#include "assert.h"

#include <ops/add.h>
#include <ops/adds.h>
#include <ops/sub.h>

#include <ops/state.h>

using namespace cheeky::ops;

int main() {
    AddImm add_imm;
    SubImm sub_imm;
    
    State state;

    state.r[0] = 0;
    state.r[8] = 0;

    int32_t bits = 0x11400900;

    assert(add_imm.is_match(bits));

    add_imm.process(bits, state);

    assert(state.r[0] == 8192);

    state.r[8] = 500;
    
    {
        int32_t sub_bits = 0x5100C908;

        assert(sub_imm.is_match(sub_bits));
    
        sub_imm.process(sub_bits, state);
        
        assert(state.r[8] == 450);
    }

    {
        int32_t sub_bits = 0x5100C908;

        assert(sub_imm.is_match(sub_bits));
    
        sub_imm.process(sub_bits, state);
        
        assert(state.r[8] == 400);
    }

    AddsImm adds_imm;

    {
        int32_t adds_bits = 0x31001508;

        assert(adds_imm.is_match(adds_bits));

        adds_imm.process(adds_bits, state);

        assert(state.r[8] == 405);
        assert(!state.nzcv.any());

        state.r[8] = 0xFFFFFFFF;

        adds_imm.process(adds_bits, state);

        assert(state.get_c_flag());
        assert(!state.get_n_flag());
        assert(!state.get_z_flag());
        assert(!state.get_v_flag());
    }

    {
        int32_t adds_bits = 0x31001508;

        state.r[8] = INT32_MAX;

        adds_imm.process(adds_bits, state);

        assert(!state.get_c_flag());
        assert(state.get_n_flag());
        assert(!state.get_z_flag());
        assert(state.get_v_flag());
    }

    return 0;
}