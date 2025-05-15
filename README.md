# Cheeky Emulator

## What's been done? 
- Mach-O files parsing module (load commands + memory segments).
- Basic ARM64 operations (arithmetic + control + load/stores + flag versions).
- ARM64 CPU core state emulation (r0-31, PC, NZCV flags, stack).
- Ops unit tests.
- Exec unit tests (entire emulation's final state validation).

## What's in progress? 
- Logging (spdlog).
- Decision tree for efficient instruction pattern matching.

## What's next?
- Dyn libs support.
- Syscalls processing, heap support.
- Output of executed instructions at runtime (asm).
- Disassembly without running, showing commands which are not currently supported.

## Building 

Emulator binary + no tests (default):

`mkdir build && cd build && cmake -S .. -B . && cmake --build . && cd ..`

Emulator binary + tests:

`mkdir build && cd build && cmake -S .. -DBUILDTESTS=TRUE -B . && cmake --build . && cd ..`

## Tests preparation

exec tests: 

`cd tests && python3 prepare_exec_tests.py && cd ..`

# Running (from project root directory)

emulator:

`./build/cheeky <filepath>`

ops tests:

`./build/tests/ops_tests`

exec tests:

`./build/tests/exec_tests`