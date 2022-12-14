# SPL
![spl_logo](https://github.com/SYXZyt/SPL/blob/v1.2/Ico/ico.png)
## _Introduction_
SPL is a scripting language developed as a first custom programming language project.
- It's syntax was mainly inspired from BASIC and X86.
- It does not feature any functions, but labels.
- Parameters can be achieved by pushing values onto a stack.

The compiler and virtual machine are all included into one visual studio solution. I developed it on Windows so I'm not 100% sure if the Linux code works, as I haven't been able to test it.
As of right now, SPL has no way to code using multiple SPL files.

You can use the `-d` flag to disassemble any input the virtual machine is given
You can use the `-b` flag to use breakpoints. See https://github.com/SYXZyt/SPL/blob/master/bytecode.md for help with breakpoints

## Variables
Variables can be updated using the `Let` keyword. This keyword will tell the virtual machine that we are wanting to assign a value in memory.
Variables in SPL can either be mutable or constant, which are created using `let` and `const` respectively. Constants are loaded into memory just before code execution begins, and cannot be changed once set. Constants can only store ints, floats or string. Copying a variable into a constant is impossible, as constants must be resolved at compile time.
Variables only have three different types. `Integer`, `Float` or `String`. These are the three basic types and in `SPL 1.0` there are no structs to create your own type.

Variables can be created using the following syntax
```
let [NAME] [VALUE]
```

Variables can not be defined as null.
Variables can be created at compile time using the const keyword.
```
const PI 3.1415
free PI ;If this is deleted, it cannot be redefined as constant
const X PI  ;Since all variables are created at compile time, constants must be able to be parsed at compile time
;Currently, this does not allow const [NAME] [ANOTHER CONST NAME] but may be added in the future
```

## Instructions
SPL features various instructions all with different functions. Many of these instructions use the stack, so it's important to understand what is on the stack and when.
| Instruction | Description |
| - | - |
| `print` [`literal` or `variable`] | Print a literal or a variable to the console |
| `println` [`literal` or `variable`] | Print something to the string followed by a newline |
| `let` [`identifier`] [`literal`] | Create a value in memory |
| `const` [`identifier` [`literal`]] | Create a constant in memory |
| `free` | Free a variable and remove its value |
| `goto` [`line_num` or `label_name`] | Jump the current execution to a specific point in the code |
| `call` [`label_name`] | Jump the current execution to a specific point, while adding the current address to a call stack. Like calling a function. Unlike `goto`, `call` cannot jump to line numbers and must be given a label name |
| `ret` | Jump the current execution to a previous point which was stored in the callstack |
| `exit` [`integer` ] | Exit the program with a specific exit code |
| `push` [`literal` or `variable`] | Push a value onto the stack |
| `pop` | Pop a value off of the stack |
| `setpop` [`identifier`] | Create a value in memory, but using the a value off the stack |
| `add` | Add two values on the stack and push the result. Will throw error if non-numeric types |
| `sub` | Subtract two values on the stack and push the result. Will throw error if non-numeric types |
| `mul` | Multiply two values on the stack and push the result. Will throw error if non-numeric types |
| `div` | Divide two values on the stack and push the result. Will throw error if non-numeric types |
| `pow` | Calculate the power of a number on the stack and push the result. Will throw error if non-numeric types |
| `concat` | Add two values on the stack as a string, and push the result |
| `to_float` | Convert a value on the stack to a float. Will throw error if invalid |
| `to_int` | Convert a value on the stack to a int. Will throw error if invalid |
| `to_string` | Convert a value on the stack to a string |
| `equ` [`line_num` or `label_name`] | Check if the current two values on the stack are equal (==), and jump to the line or label if not |
| `neq` [`line_num` or `label_name`] | Check if the current two values on the stack are not equal (!=), and jump to the line or label if not |
| `grt` [`line_num` or `label_name`] | Check if the current two values on the stack are greater (>), and jump to the line or label if not |
| `grtequ` [`line_num` or `label_name`] | Check if the current two values on the stack are greater or equal (>=), and jump to the line or label if not |
| `lwr` [`line_num` or `label_name`] | Check if the current two values on the stack are lower (<), and jump to the line or label if not |
| `lwrequ` [`line_num` or `label_name`] | Check if the current two values on the stack are lower or equal (<=), and jump to the line or label if not |
| `input` | Get the users input, and store it to the stack |
| `mod` | Calculates the mod of two numbers |
| `console setpos` | Using two values on the stack (note x is pushed first), move the cursor to a set position |
| `console clear` | Clears all text on the screen |
| `console setcolor` | Using a value on the stack, set the foreground console colour. This is how colours are implemented. [ConsoleAttributes](https://learn.microsoft.com/en-us/windows/console/console-screen-buffers#span-idwin32characterattributesspanspan-idwin32characterattributesspancharacter-attributes) |
| `sleep` [integer] | Sleep for a set amount of milliseconds |
| `random [integer]` | Pick a random number from 0 (inclusive) to the provided int (exclusive) and push the result to the stack |
| `raise` [value] | Throw a user error |
| `is_int` | Checks the current value, on the stack, can be converted to an integer, and pushes 1 onto the stack if it can be, 0 otherwise
| `is_float` | Checks the current value, on the stack, can be converted to a floating point number, and pushes 1 onto the stack if it can be, 0 otherwise

It is very important that you remember to do all maths operations in reverse. Lets say you want to do `10 / 5`, the SPL would be
```
push 5
push 10
div
```
Note that the right hand number is pushed first. This can be confusing at first and takes a little while to remember.

When moving the cursor, this is reversed with X being pushed first. So to move to x: 6, y: 2
```
push 6
push 2
console setpos
```

## Future plans
SPL has reached end of life. No official updates are planned, unless to patch major bugs.
If you wish to make any changes, feel free to do so. If deemed worthy, I'll happily accept a fork pull request.
