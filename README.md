# SPL
![spl_logo](https://github.com/SYXZyt/SPL/blob/master/Ico/ico.png)
## _Introduction_
SPL is a scripting language developed as a first custom programming language project.
- It's syntax was mainly inspired from BASIC and X86.
- It does not feature any functions, but labels.
- Parameters can be achieved by pushing values onto a stack.

The compiler and virtual machine are all included into one visual studio solution. I developed it on Windows so I'm not 100% sure if the Linux code works, as I haven't been able to test it.
As of right now, SPL has no way to code using multiple SPL files.

## Variables
Variables can be updated using the `Let` keyword. This keyword will tell the virtual machine that we are wanting to assign a value in memory.
Variables in SPL can either be mutable or constant, which are created using `let` and `const` respectively. Constants are loaded into memory just before code exeuction begins, and cannot be changed once set. Constants can only store ints, floats or string. Copying a variable into a constant is impossible, as constants must be resolved at compile time.
Variables only have three different types. `Integer`, `Float` or `String`. These are the three basic types and in `SPL 1.0` there are no structs to create your own type.

Variables can be created using the following syntax
```
let [NAME] [VALUE]
```

There is no way to create a null variable.
You also can't easily copy variables.
This is how copying variables would work
```
let dog "Woof!" ;This is the value we want to copy
let cat 0       ;This is where we want to copy into
push dog
setpop cat      ;This will take the first value on the stack, and store it to a variable called cat
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

It is very important that you remember to do all maths operations in reverse. Lets say you want to do `10 / 5`, the SPL would be
```
push 5
push 10
div
```
Note that the right hand number is pushed first. This can be confusing at first and takes a little while to remember.

## Known Issues
It is possible to execute unwanted code if a label is not at the start of the line
E.g.
```
1:  goto label ;<- Since the preprocessor converts this to 'goto 2', the code on line 2 before the label, will execute
2:  println "I shouldn't execute'" :label println "I should execute"
3:  exit
4:
5:  ;Expected output
6:  ;"I should execute"
7:
8:  ;Actual output
9:  ;"I shouldn't execute"
10: ;"I should execute"
```

## Future Plans
**SPL 1.2** plans to add more advanced to more advanced console control, such as moving the cursor and coloured text. Both _British_ and _American_ spellings of "Colour" will be supported\
**SPL 1.2** is planned to be the final officially supported version of SPL, as my focus will move onto a much larger and more advanced language framework. The code will remain open source if anyone wants to update the language