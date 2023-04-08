# fe16-vm

fe16-vm is simple 16bit virtual machine based on RISC architecture. 

## Opcodes

| Opcode  | Description           |
|:-------:|-----------------------|
| mov     | Move                  |
| add     | Add                   |
| sub     | Sub                   |
| and     | Logical AND           |
| xor     | Logical XOR           |
| push    | Push to stack         |
| pop     | Pop from stack        |
| jmp     | Jump to line          |
| cmp     | Compares two operands |
| jme     | Jump to line if equal |
| sys     | System call           |
| call    | Call procedure        |
| ret     | Return from procedure |


## Registers

| Register | Description           |
|:--------:|-----------------------|
| f0       |                       |
| f1       |                       |
| f2       |                       |
| f3       |                       |
| f4       |                       |
| fsp      | Stack pointer         |
| fcd      | Last operation status |
| fpc      | Program counter       |

fe16-vm has only 8 registers. Of these, only 5 are for general purpose  ( `f0` ... `f4` ). Other 3 registers for program counter, storing flags and stack pointer. 

## System calls

| Syscall Name | f0             | f1             | f2 | f3 | f4  |
|--------------|:--------------:|:--------------:|:--:|:--:|:---:|
| exit         | exit code      | -              | -  | -  | 0x1 |
| write        | file descriptor| string length  | -  | -  | 0x2 |
| read         | file descriptor| string length  | -  | -  | 0x3 |
