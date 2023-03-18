# fe16-vm

fe16-vm is simple 16bit virtual machine based on RISC architecture. 

## Opcodes

| Opcodes | Description |
|---------|-------------|
| mov     | Move        |
| add     | Add         |
| sub     | Sub         |
| and     | Logical AND |
| xor     | Logical XOR |


## Registers

fe16-vm has only 8 registers. Of these, only 6 are for general purpose ( `f0` ... `f5` ). Other 2 registers for program pointer and storing flags. 
