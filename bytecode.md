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
| 0F | `pushstr` | Push a string to the stack |
| 10 | `pushflt` | Push a float to the stack |
| 11 | `pushint` | Push an integer to the stack |
| 12 | `pushvar` | Push a variable to the stack |
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
| 1D | `equ` | Run a `==` comparison |
| 1E |`neq` | Run a `!=` comparison |
| 1F |`grt` | Run a `>` comparison |
| 20 |`grtequ` | Run a `>=` comparison |
| 21 |`lwr` | Run a `<` comparison |
| 22 |`lwrequ` | Run a `<=` comparison |
| 23 | `inc` | Increment an int or float on the stack |
| 24 | `dec` | Decrement an int or float on the stack |
| 25 | `input` | Get the users input and store it on the stack |
| 26 | `mod` | Calculates the mod of two numbers |
| 27 | `console setpos` | Sets the position of the console cursor, using two values on the stack. Note that x is pushed first |
| 28 | `console clear` | Clears all text on the console |
| 29 | `console setcolor` | Sets the foreground of the console |
| 2A | `sleep` | Sleep for a specified amount of milliseconds |
| 2B | `random` | Generates a random number between 0 and an int on the stack |

## Breakpoints
Running a program with the -b flag will force a break point. The breakpoint system may look confusing, but it is simple to read once you understand it.
It can be broken into sections.

First of all, here is a sample breakpoint dump
```
Breakpoint Disassembly (see bytecode.md for help reading)
Stored Identifiers:
[0] | WIDTH
[1] | HEIGHT
[2] | i
[3] | x
[4] | y

Stored Variables:
HEIGHT | 30 (INT)
WIDTH | 119 (INT)
i | 48 (INT)
x | 2 (INT)
y | 0 (INT)

Stack (1) Bottom Value : 255 (INT)

Address: 0030

VM Code:
12 |  02 00 00 00

Disassembled SPL:
varpush [2]
Press [ENTER] to step through
```

At the very top is `Stored Identifiers`. This is all of the variable names used in your program. SPL doesn't store variable names in the code, but rather an offset.
This is done, so went you want to write to a variable with a name of 20 characters, those 20 characters are read before execution, and when referencing the variable,
only the 4 byte offset needs reading. This is done for performance reasons, but it also means that SPL can only store 2,147,483,647 unique variable names. It's very unlikely anyone will ever reach that limit though
Notice when you delete a variable, it's identifier is never deleted.

Next is `Stored Variables`. This is where the actual values of your variables are. It's pretty easy to read. It follows this format: `NAME | VALUE (TYPE)`

Next up, is the stack. Due to how stacks work, we can only print the bottom or top value, depending which way you visualise a stack. I prefer bottom, but either way, it's the same as stack.Peek()

`Address` refers to the address of the current byte we are reading. Note that this isn't the same as the value would be in the rom, as the rom contains identifier and constant data as well.
Address can be handy for when debugging function calls and such.

`VM Code` simply shows all of the bytes that make up the current instruction. The byte before the pipe is the opcode. Use the table above to see what the opcode is, and how the following bytes make up the instruction

The final section just shows the decompiled instruction. Unfortunately not all instructions can be perfectly recreated, such as calls and gotos, as labels and line numbers are replaced by their byte offset during assembly.
The disassembled instruction should be enough to understand what is being executed.
When an instruction has a number in square brackets, it is an identifer offset. So the instruction in the example is this:
`varpush [2]` -> `varpush identifer 2` -> `varpush i`, which in SPL is `push i`

Disassembled instructions may have a different name to SPL instructions. This is just to tell similar instructions apart.
For example `let i j` disassembled would be `cpy [0] [1]` for example.