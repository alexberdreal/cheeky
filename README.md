# Cheeky Emulator

## What is done? 
- Mach-O files parsing module (load commands + memory segments).
- Basic ARM64 operations (add(s), sub(s), movz, str, ldr, ret, orr).
- ARM64 CPU core state emulation (r0-31, PC, NZCV flags, stack).
- Google tests of ops module.

## What should be done in the near future? 
- Jumps support, proper frame ptr/link register updates + tests.
- Functions linking, dynamic libs loading.
- Syscalls processing, heap support (malloc has to be redirected by emulator's process).
- Output of executed instructions at runtime (asm).
- Disassembly without running, showing commands which are not currently supported.

## Building 

Emulator binary + no tests (default):

`mkdir build && cd build && cmake -S .. -B . && cmake --build .`

Emulator binary + tests:

`mkdir build && cd build && cmake -S .. -DBUILDTESTS=TRUE -B . && cmake --build .`

# Running 

emulator:

`./build/cheeky <filepath>`

ops tests:

`./build/tests/ops_test`
