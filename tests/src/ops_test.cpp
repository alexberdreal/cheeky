#include "assert.h"

#include <gtest/gtest.h> 

#include <ops/add.h>
#include <ops/adds.h>
#include <ops/sub.h>

#include <core/state.h>

using namespace cheeky::ops;
using namespace cheeky::core;

TEST(OpsTest, AddImm_test) {
    AddImm add_imm;
    State state;

    state.get_r_ref(0) = 0;
    state.get_r_ref(8) = 0;

    int32_t bits = 0x11400900; // add x0, x8, #0x2 lsl 12

    assert(add_imm.is_match(bits));

    add_imm.process(bits, state);

    assert(state.get_r_ref(0) == 8192);
}

TEST(OpsTest, SubImm_test) {
    SubImm sub_imm;
    State state;

    state.get_r_ref(8) = 500;

    int32_t bits = 0x5100C908; // sub x8, x8, #0x32

    assert(sub_imm.is_match(bits));

    sub_imm.process(bits, state);

    assert(state.get_r_ref(8) == 450);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}