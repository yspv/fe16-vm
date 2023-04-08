#ifndef FE16_OPCODES_H
#define FE16_OPCODES_H

typedef enum {
    mov = 0,     /*  0x0  move                   */
    add,         /*  0x1  add                    */
    sub,         /*  0x2  subtract               */
    jmp,         /*  0x3  jump to line           */
    push,        /*  0x4  push to stack          */
    pop,         /*  0x5  pop stack              */
    xor,         /*  0x6  logical xor            */
    and,         /*  0x7  logical and            */
    sys,         /*  0x8  syscall                */
    cmp,         /*  0x9  compares two operands  */
    jme,         /*  0xA  jump if equal          */
    call,        /*  0xB  call function          */
    ret,         /*  0xC  return from function   */
    op_count
} Fe16_opcode;

#endif
