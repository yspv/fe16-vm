#ifndef FE16VM_H
#define FE16VM_H
#define MEMORY_SIZE 1 << 16

/* typedefs */
typedef enum {
    f0 = 0,       /* 00 */
    f1,           /* 01 */
    f2,           /* 02 */
    f3,           /* 03 */
    f4,           /* 04 */
    f5,           /* 05 */
    fcd,          /* 06 */
    fip,          /* 07 */
    reg_count,
} Fe16_reg;

typedef enum {
    mov = 0,     /* 0x0 */
    add,         /* 0x1 */
    sub,         /* 0x2 */
    jmp,         /* 0x3 */
    push,        /* 0x4 */
    pop,         /* 0x5 */
    xor,         /* 0x6 */
    and,         /* 0x7 */
    op_count
} Fe16_opcode;

typedef enum {
    zf,           /* zefo flag      */
    nf,           /* negative flag  */
    of,           /* overflow flag  */
} Fe16_flag;  

#endif
