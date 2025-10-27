# SPU

Here is my project called SPU. In fact, It is an interpreter written in C language with ISA based on asm.

This SPU works with [Stack](https://github.com/4locker4/Stack.git) and can push/pop to/form memory.

Since SPU was written when I first started programming, it is not a RISC or CISC implementation. All commands interact only with the stack or memory, it is impossible to directly, for example, add register to register.

### How it works?

First, a block called "assembler" reads commands from a text file and encodes them. It then writes the encoded commands to a binary file. The first two numbers in the binary file are the signature, and the third number is the total number of commands.
The "SPU" block then reads the binary file into a string, verifying the signature and reading the number of commands. The SPU decodes and executes the command in one iteration of the loop, simulating the operation of the interpreter.

### Command Codes and Mnemonics

| Command Code | Mnemonic | Description | Usage Example |
|-------------|----------|-------------|---------------|
| `0` | `hlt` | Halt processor | `hlt` |
| `1` | `push` | Push to stack | `push 5` |
| `2` | `pop` | Pop from stack | `pop rax` |
| `3` | `add` | Addition | `add` |
| `4` | `sub` | Subtraction | `sub` |
| `5` | `mul` | Multiplication | `mul` |
| `6` | `div` | Division | `div` |
| `7` | `outp` | Data output | `outp` |
| `8` | `inp` | Data input | `inp` |
| `9` | `sqrt` | Square root | `sqrt` |
| `10` | `sin` | Sine function | `sin` |
| `11` | `cos` | Cosine function | `cos` |
| `12` | `dump` | CPU state dump | `dump` |
| `13` | `jmp` | Unconditional jump | `jmp label` |
| `14` | `ja` | Jump if above (unsigned) | `ja label` |
| `15` | `jae` | Jump if above or equal (unsigned) | `jae label` |
| `16` | `jb` | Jump if below (unsigned) | `jb label` |
| `17` | `jbe` | Jump if below or equal (unsigned) | `jbe label` |
| `18` | `je` | Jump if equal | `je label` |
| `19` | `jne` | Jump if not equal | `jne label` |
| `20` | `call` | Subroutine call | `call func` |
| `21` | `ret` | Return from subroutine | `ret` |

## Special Codes

| Code | Description |
|------|-------------|
| `-1` | Command not initialized |

## Usage Examples

```asm
; Stack operations
push 10      ; code 1
push 20      ; code 1
add          ; code 3
pop rax      ; code 2

; Arithmetic operations
push 5       ; code 1
push 3       ; code 1
sub          ; code 4 - result: 2

; Conditional jumps
push 10      ; code 1
push 20      ; code 1
jb less_label    ; code 16 - jump if 10 < 20

; Function calls
call my_function ; code 20
ret              ; code 21

; Mathematical functions
push 90      ; code 1
sin          ; code 10 - sine of 90 degrees/radians

; System commands
dump         ; code 12
hlt          ; code 0
```
