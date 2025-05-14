#include "assert.h"
#include <gtest/gtest.h>
#include <ops/ops.h>
#include <core/state.h>

using namespace cheeky::ops;
using namespace cheeky::core;

TEST(OpsTest, AddImm) {
    AddImm op;
    State state;
    
    // Test: ADD X0, X8, #0x2 LSL 12 (8192)
    state.get_r_ref_64(0) = 0;
    state.get_r_ref_64(8) = 0;
    const uint32_t instr = 0x11400900;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_64(0), 8192) << "X0 should contain 8192 after shifted add";
}

TEST(OpsTest, SubImm) {
    SubImm op;
    State state;
    
    // Test: SUB X8, X8, #0x32 (50)
    state.get_r_ref_64(8) = 500;
    const uint32_t instr = 0x5100C908;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_64(8), 450) << "X8 should be 450 after subtraction";
}

TEST(OpsTest, SubsImm) {
    SubsImm op;
    State state;
    
    // Test: SUBS X8, X8, #0x32 (50)
    state.get_r_ref_64(8) = 500;
    const uint32_t instr = 0x7100C908;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    
    ASSERT_EQ(state.get_r_ref_64(8), 450) << "Register value mismatch";
    ASSERT_EQ(state.get_n_flag(), 0) << "Negative flag incorrect";
    ASSERT_EQ(state.get_z_flag(), 0) << "Zero flag incorrect";
    ASSERT_EQ(state.get_c_flag(), 1) << "Carry flag incorrect";
    ASSERT_EQ(state.get_v_flag(), 0) << "Overflow flag incorrect";
}

TEST(OpsTest, OrrShMove) {
    OrrSh op;
    State state;
    
    // Test: ORR X8, X0, #0 (Move X0 to X8)
    state.get_r_ref_64(0) = 0x1234;
    state.get_r_ref_64(8) = 0xFFFF;
    const uint32_t instr = 0xAA0003E8;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    
    ASSERT_EQ(state.get_r_ref_64(8), 0x1234) << "X8 should match X0 value";
    ASSERT_EQ(state.get_r_ref_64(0), 0x1234) << "X0 should remain unchanged";
}

TEST(OpsTest, OrrShLSL) {
    OrrSh op;
    State state;
    
    // Test: ORR X9, X2, X3, LSL #4
    state.get_r_ref_64(2) = 0b00001;
    state.get_r_ref_64(3) = 0b00001;
    const uint32_t instr = 0xAA031049;
    
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_64(9), 0b10001) << "LSL operation result mismatch";
}

TEST(OpsTest, OrrShLSR) {
    OrrSh op;
    State state;
    
    // Test: ORR X9, X2, X3, LSR #3
    state.get_r_ref_64(2) = 0b10000;
    state.get_r_ref_64(3) = 0b11000;
    const uint32_t instr = 0xAA430C49;
    
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_64(9), 0b10011) << "LSR operation result mismatch";
}

TEST(OpsTest, OrrShASR) {
    OrrSh op;
    State state;
    
    // Test: ORR X9, X2, X3, ASR #2
    state.get_r_ref_64(2) = 0b00000;
    state.get_r_ref_64(3) = 0b10000 | (uint64_t(1) << 63);
    const uint32_t instr = 0xAA830849;
    
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_64(9), (uint64_t(0b111) << 61) | 0b00100) 
        << "ASR operation result mismatch";
}

TEST(OpsTest, Movz) {
    Movz op;
    State state;
    
    // Test: MOVZ W0, #0
    state.get_r_ref_64(0) = 0xFFFF;
    const uint32_t instr = 0x52800000;
    
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_64(0), 0) << "W0 should be zero after MOVZ";
}

TEST(OpsTest, StrImm) {
    StrImm op;
    State state;
    
    // Test: STR W8, [SP, #0x8]
    state.get_r_ref_32(8) = 0xFFFF;
    state.get_r_ref_32(31) = 0;  // SP
    auto& vm = state.get_vm_with_offset_32(0x8);
    const uint32_t instr = 0xB9000BE8;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    ASSERT_EQ(vm, 0xFFFF) << "Memory should contain 0xFFFF";
}

TEST(OpsTest, LdrImm) {
    LdrImm op;
    State state;
    
    // Test: LDR W8, [SP, #0xC]
    state.get_r_ref_32(31) = 0;  // SP
    auto& vm = state.get_vm_with_offset_32(0xC);
    vm = 0xABC;
    const uint32_t instr = 0xB9400FE8;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_32(8), 0xABC) << "W8 should load 0xABC";
}

TEST(OpsTest, Stur) {
    Stur op;
    State state;
    
    // Test: STUR WZR, [X29, #-0x4]
    state.get_r_ref_64(29) = 0xFF;
    auto& vm = state.get_vm_with_offset_32(0xFB);  // 0xFF - 0x4
    vm = 0xABC;
    const uint32_t instr = 0xB81FC3BF;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    ASSERT_EQ(vm, 0) << "Memory should be zero after storing WZR";
}

TEST(OpsTest, Stp) {
    Stp op;
    State state;
    
    // Test: STP X29, X30, [SP, #0x10]
    state.get_r_ref_64(29) = 0xFA;
    state.get_r_ref_64(30) = 0xAB;
    state.get_r_ref_64(31) = 0xFF;  // SP
    const uint32_t instr = 0xA9017BFD;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    
    auto& vm1 = state.get_vm_with_offset_64(0xFF + 0x10);
    auto& vm2 = state.get_vm_with_offset_64(0xFF + 0x18);
    ASSERT_EQ(vm1, 0xFA) << "First stored value mismatch";
    ASSERT_EQ(vm2, 0xAB) << "Second stored value mismatch";
}

TEST(OpsTest, Ldur) {
    Ldur op;
    State state;
    
    // Test: LDUR W8, [X29, #-0x4]
    state.get_r_ref_64(29) = 0xFF;
    state.get_vm_with_offset_32(0xFB) = 0xABC;  // 0xFF - 0x4
    const uint32_t instr = 0xB85FC3A8;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    ASSERT_EQ(state.get_r_ref_32(8), 0xABC) << "W8 should load 0xABC";
}

TEST(OpsTest, Ldp) {
    Ldp op;
    State state;
    
    // Test: LDP X29, X30, [SP, #0x10]
    state.get_r_ref_64(31) = 0xFF;  // SP
    state.get_vm_with_offset_64(0xFF + 0x10) = 0xFA;
    state.get_vm_with_offset_64(0xFF + 0x18) = 0xAB;
    const uint32_t instr = 0xA9417BFD;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    
    ASSERT_EQ(state.get_r_ref_64(29), 0xFA) << "X29 load mismatch";
    ASSERT_EQ(state.get_r_ref_64(30), 0xAB) << "X30 load mismatch";
}

TEST(OpsTest, Adrp) {
    Adrp op;
    State state;
    
    // Test: adrp	x9, 1 ;   
    uint64_t vm_addr = 0x100003f68;
    state.update_pc(vm_addr / 4);
    const uint32_t instr = 0xb0000009;
    
    ASSERT_TRUE(op.is_match(instr));
    op.process(instr, state);
    
    ASSERT_EQ(state.get_r_ref_64(9), 0x100004000 / 4) << "X29 load mismatch";
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
