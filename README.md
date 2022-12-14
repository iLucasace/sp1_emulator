# Simple Processor 1 Emulator

## Basic Information

SP1 is a hypothetical processor specified for purely academic purposes. It has three registers of 8 bits (1 byte) each:

- acc (accumulator)
- stat (status) 
- pc (program counter)

The SP1 processor ISA consists of 20 instructions. The format of the instructions is fixed, in this case 16 bits (8 bits for the opcode and 8 bits for the operand). The stat register holds carry, overflow and zero information from the acc register.

### Create the assembler executable:
```
$ cd ./SP1Assembler
$ make
```

### Assemble the .asm program to .bin:
```
$ ./SP1Assembler/sp1a <file_name>.asm -o <binary_file_name>.bin
```

### Run the emulator:
```
$ gcc sp1.c -o sp1.exe
$ ./sp1.exe <binary_file_name>.bin
```
