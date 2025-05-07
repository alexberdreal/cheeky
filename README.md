# Cheeky Emulator

## What is done? 
- Mach-O files parsing module (load commands + memory segments).
- Basic ARM64 operations (add, adds, sub).
- ARM64 CPU core state emulation (r0-31, PC, NZCV flags).
- Simple tests of supported 4-byte instructions processing (updating general-purpose registers and NZCV flags). 

## What should be done in the near future? 
- Emulated memory, functions linking, dinamic libs loading.
- Other ARM64 instructions.
- Output of executed instructions at runtime (asm).
- Proper tests using Google Tests.