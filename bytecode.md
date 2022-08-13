# Byte Code Instructions and the Virtual Machine
## Introduction
In order to keep speed higher, SPL will compiler into a bytecode. The bytes will be loaded into a virtual machine which will read the bytes and execute instructions. Like a CPU but much higher level.
The virtual machine has little information to throw errors so all of the errors it does throw are rather basic. `Unknown Opcode 0x45` for example.
## Opcodes
Opcodes are the bytes that tell the VM what instruction to execute. Opcodes may be similar to each other, but their value denotes additional information. For example `0x01` could be print string, but `0x02` could be print a variable
Something to note is that the disassembly may 

| Opcode | Opcode Name | Description |
| - | - | - |
| 00 | `nop` | Does nothing. Is ignored by the VM |
| 01 | `setpop` | Gets a value off of the stack, and stores it into a variable
| 02 | `str`| Creates a string variable |
| 03 | `flt`| Creates a float variable |
| 04 | `int` | Creates an integer variable |
| 05 | `cpy` | Copies the value of another variable |
| 06 | `strout` | Prints a string to the console |
| 07 | `fltout` | Prints a float to the console |
| 08 | `intout` | Prints an integer to the console |
| 09 | `varout` | Prints a variable to the console |
| 0A | `free` | Deletes a variable from memory |
| 0B | `exit` | Exits code execution |
| 0C | `goto` | Jumps the program counter to a specific address |
| 0D | `call` | Jumps the program counter to a specific address, while adding the current address to the callstack |    
| 0E | `ret` | Jumps to the latest address saved on the call stack |
| 0F | `push` (string) | Push a string to the stack |
| 10 | `push` (float) | Push a float to the stack |
| 11 | `push` (int) | Push an integer to the stack |
| 12 | `push` (variable) | Push a variable to the stack |
| 13 | `pop` | Pop a value off of the stack |
| 14 | `add` | Add two values together off of the stack, and push the result |
| 15 | `sub` | Subtract two values together off of the stack, and push the result |
| 16 | `mul` | Multiply two values together off of the stack, and push the result |
| 17 | `div` | Divide two values together off of the stack, and push the result |
| 18 | `pow` | Calculate the power of two values off of the stack, and push the result. Bottom number on stack will be the base |
| 19 | `concat` | Add two values together, as strings, off of the stack, and push the result |
| 1A | `strcast` | Take a value off of the stack, convert it to a string, and then push it |
| 1B | `fltcast` | Take a value off of the stack, convert it to a float, and then push it |
| 1C | `intcast` | Take a value off of the stack, convert it to an integer, and then push it |