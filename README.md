# Cheeky Emulator

## What is done? 
- Mach-O files parsing module (load commands + memory segments).
- Basic ARM64 operations (add, adds, sub).
- ARM64 CPU core state emulation (r0-31, PC, NZCV flags).
- Google tests of ops module.

## What should be done in the near future? 
- Emulated memory, functions linking, dinamic libs loading.
- Other ARM64 instructions.
- Output of executed instructions at runtime (asm).

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
