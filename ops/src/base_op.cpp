#include <core/state.h>
#include <ops/ops.h>

namespace cheeky::ops {
bool handle_instruction(uint32_t instruction, core::State& state) {
switch (instruction & 0x3c000000) {
case 0x10000000: {
switch (instruction & 0x7f800000) {
case 0x11000000: {
return AddImm::process(instruction, state);
}
case 0x51000000: {
return SubImm::process(instruction, state);
}
case 0x52800000: {
return Movz::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0x30000000: {
switch (instruction & 0x7f800000) {
case 0x31000000: {
return AddsImm::process(instruction, state);
}
case 0x71000000: {
return SubsImm::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0x14000000: {
switch (instruction & 0xfc000000) {
case 0x14000000: {
return B::process(instruction, state);
}
case 0x94000000: {
return Bl::process(instruction, state);
}
case 0xd4000000: {
return Ret::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0x18000000: {
return Csinc::process(instruction, state);
}
case 0x34000000: {
return Tbnz::process(instruction, state);
}
case 0x28000000: {
switch (instruction & 0x7f000000) {
case 0x2a000000: {
return OrrSh::process(instruction, state);
}
case 0x28000000: {
switch (instruction & 0x7fc00000) {
case 0x28800000: {
return Stp::process(instruction, state);
}
case 0x28c00000: {
return Ldp::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0x29000000: {
switch (instruction & 0x7fc00000) {
case 0x29800000: {
return Stp::process(instruction, state);
}
case 0x29000000: {
return Stp::process(instruction, state);
}
case 0x29c00000: {
return Ldp::process(instruction, state);
}
case 0x29400000: {
return Ldp::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0x38000000: {
switch (instruction & 0xbfc00000) {
case 0xb8000000: {
switch (instruction & 0xbfe00c00) {
case 0xb8000400: {
return StrImm::process(instruction, state);
}
case 0xb8000c00: {
return StrImm::process(instruction, state);
}
case 0xb8000000: {
return Stur::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0xb9000000: {
return StrImm::process(instruction, state);
}
case 0xb8400000: {
switch (instruction & 0xbfe00c00) {
case 0xb8400400: {
return LdrImm::process(instruction, state);
}
case 0xb8400c00: {
return LdrImm::process(instruction, state);
}
case 0xb8400000: {
return Stur::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
case 0xb9400000: {
return LdrImm::process(instruction, state);
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
default: {
std::cerr << "Instruction is not supported: " << std::hex << instruction;
return false;
}
}
}
}
