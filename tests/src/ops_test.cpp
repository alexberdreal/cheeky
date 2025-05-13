#include "assert.h"

#include <gtest/gtest.h> 

#include <ops/ops.h>
#include <core/state.h>

using namespace cheeky::ops;
using namespace cheeky::core;

TEST(OpsTest, AddImm_test) {
    AddImm add_imm;
    State state;

    state.get_r_ref_64(0) = 0;
    state.get_r_ref_64(8) = 0;

    int32_t bits = 0x11400900; // add x0, x8, #0x2 lsl 12

    ASSERT_TRUE(add_imm.is_match(bits));

    add_imm.process(bits, state);

    ASSERT_EQ(state.get_r_ref_64(0), 8192);
}

TEST(OpsTest, SubImm_test) {
    SubImm sub_imm;
    State state;

    state.get_r_ref_64(8) = 500;

    int32_t bits = 0x5100C908; // sub x8, x8, #0x32

    ASSERT_TRUE(sub_imm.is_match(bits));

    sub_imm.process(bits, state);

    ASSERT_EQ(state.get_r_ref_64(8), 450);
}

TEST(OpsTest, SubsImm_test) {
    SubsImm subs_imm;
    State state;

    // Initialize register x8 to 500
    state.get_r_ref_64(8) = 500;

    // Instruction bits for: subs x8, x8, #0x32 (subtract immediate with flags)
    int32_t bits = 0x7100C908;

    // Check if instruction matches SubsImm
    ASSERT_TRUE(subs_imm.is_match(bits));

    // Process the instruction
    subs_imm.process(bits, state);

    // After subtraction: 500 - 50 = 450
    ASSERT_EQ(state.get_r_ref_64(8), 450);

    // N flag: 0 (result positive)
    ASSERT_EQ(state.get_n_flag(), 0);
    // Z flag: 0 (result not zero)
    ASSERT_EQ(state.get_z_flag(), 0);
    // C flag: 1 (no borrow, 500 >= 50)
    ASSERT_EQ(state.get_c_flag(), 1);
    // V flag: 0 (no signed overflow)
    ASSERT_EQ(state.get_v_flag(), 0);
}

TEST(OpsTest, OrrSh_test) {
    OrrSh orr_sh;
    State state;

    // Initialize source register x0 with test value
    state.get_r_ref_64(0) = 0x1234;
    // Initialize destination register x8 with different value
    state.get_r_ref_64(8) = 0xFFFF;

    // Instruction bits for: orr x8, x0, #0 (mov x8, x0)
    int32_t bits = 0xAA0003E8;

    // Verify instruction matching
    ASSERT_TRUE(orr_sh.is_match(bits));

    // Process the instruction
    orr_sh.process(bits, state);

    // Verify register transfer
    ASSERT_EQ(state.get_r_ref_64(8), 0x1234);  // x8 should now equal x0's original value
    ASSERT_EQ(state.get_r_ref_64(0), 0x1234);  // x0 should remain unchanged
}

TEST(OpsTest, OrrSh_LSL) {
    OrrSh orr_sh;
    State state;

    // ORR x9, x2, x3, LSL #4
    state.get_r_ref_64(2) = 0b00001;
    state.get_r_ref_64(3) = 0b00001;
    orr_sh.process(0xAA031049, state); // Encoding for shift=4, LSL
    
    // Expected: 0x0000FFFF0000FFFF | (0x1234567812345678 << 4)
    ASSERT_EQ(state.get_r_ref_64(9), 0b10001);
    ASSERT_EQ(state.get_r_ref_64(2), 0b00001); // Verify source unchanged
}

TEST(OpsTest, OrrSh_LSR) {
    OrrSh orr_sh;
    State state;

    // ORR x9, x2, x3, LSL #3
    state.get_r_ref_64(2) = 0b10000;
    state.get_r_ref_64(3) = 0b11000;
    orr_sh.process(0xAA430C49, state); // Encoding for shift=4, LSL
    
    // Expected: 0x0000FFFF0000FFFF | (0x1234567812345678 << 4)
    ASSERT_EQ(state.get_r_ref_64(9), 0b10011);
    ASSERT_EQ(state.get_r_ref_64(2), 0b10000); // Verify source unchanged
}

TEST(OpsTest, OrrSh_ASR) {
    OrrSh orr_sh;
    State state;

    // ORR x9, x2, x3, ASR #2
    state.get_r_ref_64(2) = 0b00000;
    state.get_r_ref_64(3) = 0b10000 | (uint64_t(1) << 63);
    orr_sh.process(0xAA830849, state); // Encoding for shift=4, LSL
    
    // Expected: 0x0000FFFF0000FFFF | (0x1234567812345678 << 4)
    ASSERT_EQ(state.get_r_ref_64(9), (uint64_t(0b111) << 61) | 0b00100);
    ASSERT_EQ(state.get_r_ref_64(2), 0b00000); // Verify source unchanged
}

TEST(OpsTest, Movz_test) {
    Movz orr_sh;
    State state;

    // mov	w0, #0x0
    state.get_r_ref_64(0) = 0xFFFF;
    orr_sh.process(0x52800000, state); // Encoding for shift=4, LSL

    ASSERT_EQ(state.get_r_ref_64(0), 0);
}

TEST(OpsTest, StrImm_test) {
    StrImm str_imm;
    State state;

    // str	w8, [sp, #0x8]

    ASSERT_TRUE(str_imm.is_match(0xb9000be8));

    // mov	w0, #0x0
    state.get_r_ref_32(8) = 0xFFFF;
    state.get_r_ref_32(31) = 0;
    auto& wsp = state.get_r_ref_32(31);
    auto& vm = state.get_vm_with_offset_32(wsp + 0x8);
    vm = 0;
    str_imm.process(0xb9000be8, state); 

    ASSERT_EQ(vm, 0xFFFF);
}

TEST(OpsTest, LdrImm_test) {
    LdrImm ldr_imm;
    State state;

    uint32_t instr = 0xB9400FE8;

    // ldr	w8, [sp, #0xc]
    ASSERT_TRUE(ldr_imm.is_match(instr));

    state.get_r_ref_32(31) = 0;

    auto& w8 = state.get_r_ref_32(8);
    auto& wsp = state.get_r_ref_32(31);
    auto& vm = state.get_vm_with_offset_32(wsp + 0xc);
    w8 = 0;
    vm = 0xABC;
    ldr_imm.process(instr, state); 

    ASSERT_EQ(w8, 0xABC);
}

TEST(OpsTest, Stur_test) {
    Stur stur;
    State state;

    // stur	wzr, [x29, #-0x4]

    ASSERT_TRUE(stur.is_match(0xb81fc3bf));

    // mov	x29, #0xFF
    state.get_r_ref_32(29) = 0xFF;

    auto& vm = state.get_vm_with_offset_32(0xFF - 0x4);

    vm = 0xABC;

    stur.process(0xb81fc3bf, state); 

    ASSERT_EQ(vm, 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}