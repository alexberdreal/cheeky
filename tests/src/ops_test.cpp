#include "assert.h"

#include <gtest/gtest.h> 

#include <ops/ops.h>

#include <core/state.h>

using namespace cheeky::ops;
using namespace cheeky::core;

TEST(OpsTest, AddImm_test) {
    AddImm add_imm;
    State state;

    state.get_r_ref(0) = 0;
    state.get_r_ref(8) = 0;

    int32_t bits = 0x11400900; // add x0, x8, #0x2 lsl 12

    ASSERT_TRUE(add_imm.is_match(bits));

    add_imm.process(bits, state);

    ASSERT_EQ(state.get_r_ref(0), 8192);
}

TEST(OpsTest, SubImm_test) {
    SubImm sub_imm;
    State state;

    state.get_r_ref(8) = 500;

    int32_t bits = 0x5100C908; // sub x8, x8, #0x32

    ASSERT_TRUE(sub_imm.is_match(bits));

    sub_imm.process(bits, state);

    ASSERT_EQ(state.get_r_ref(8), 450);
}

TEST(OpsTest, SubsImm_test) {
    SubsImm subs_imm;
    State state;

    // Initialize register x8 to 500
    state.get_r_ref(8) = 500;

    // Instruction bits for: subs x8, x8, #0x32 (subtract immediate with flags)
    int32_t bits = 0x7100C908;

    // Check if instruction matches SubsImm
    ASSERT_TRUE(subs_imm.is_match(bits));

    // Process the instruction
    subs_imm.process(bits, state);

    // After subtraction: 500 - 50 = 450
    ASSERT_EQ(state.get_r_ref(8), 450);

    // N flag: 0 (result positive)
    ASSERT_EQ(state.get_n_flag(), 0);
    // Z flag: 0 (result not zero)
    ASSERT_EQ(state.get_z_flag(), 0);
    // C flag: 1 (no borrow, 500 >= 50)
    ASSERT_EQ(state.get_c_flag(), 1);
    // V flag: 0 (no signed overflow)
    ASSERT_EQ(state.get_v_flag(), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}