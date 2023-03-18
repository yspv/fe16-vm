/* ____________***________
 * | PPPP DDDM NNNN NNNN |
 * -----------------------
 *             NNNN NNNN - Store a value. 
 *
 *           M - MODE bit. If mode bit 1 other N bits store a number else
 *                           first 3 N bits store register address.
 *
 *        DDD - Destention bits. Store a destention register address.
 *
 *  PPPP - Opcode bits.
 *
 *------------------------------------------------------------------------
 *     
 *    
 *   
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define MEMORY_SIZE 1 << 16


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
    mov,         /* 0x0 */
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
    zf,           /* zefo flag */
    nf,           /* negative flag */
    of,           /* overflow flag  */
} Fe16_flag;  

uint16_t fe16_memory[MEMORY_SIZE];
uint16_t fe16_regs[reg_count];

static uint16_t fe16_get_mode(uint16_t op, uint16_t inst)
{
    uint16_t imm = ((op << 12) ^ inst);
    if(imm > (imm ^ 0x100)) {
        return 1;                         /* Immadiate mode */
    }
    return 0;                              /* Register mode */
}

static uint16_t fe16_get_opcode(uint16_t inst)
{
    return inst >> 12;
}

/* For get destention registor */
static uint16_t fe16_get_dreg(Fe16_opcode op, uint16_t inst)
{
    return ((op << 12) ^ inst) >> 9;
}

static int16_t fe16_get_src(uint16_t inst)
{
    return ((inst >> 8) << 8) ^ inst;
}

static void fe16_update_flags(Fe16_reg reg)
{
    if(fe16_regs[reg] == 0) {
        fe16_regs[fcd] = zf;
    } 
    else if(fe16_regs[reg] >> 15) {
        fe16_regs[fcd] = nf;
    }
}

void fe16_inst_mov(uint16_t reg, uint16_t src)
{ 
    fe16_regs[reg] = src;  
}

void fe16_inst_add(uint16_t reg, uint16_t src)
{
    fe16_regs[reg] += src;
}

void fe16_inst_sub(uint16_t reg, uint16_t src)
{
    fe16_regs[reg] -= src;
}

void fe16_inst_and(uint16_t reg, uint16_t src)
{
    fe16_regs[reg] &= src;
}

void fe16_inst_xor(uint16_t reg, uint16_t src)
{
    uint16_t tmp = fe16_regs[reg];
    fe16_regs[reg] = tmp ^ src;
}

void fe16_inst_execute(uint16_t inst)
{
    uint16_t op   = fe16_get_opcode(inst);
    uint16_t reg  = fe16_get_dreg(op, inst);
    uint16_t mode = fe16_get_mode(op, inst);
    uint16_t src  = fe16_get_src(inst);
    switch(op) {
    case mov:
        if(mode)
            fe16_inst_mov(reg, src);
        else
            fe16_inst_mov(reg, fe16_regs[src>>4]);
        break;
    case add:
        if(mode)
            fe16_inst_add(reg, src);
        else
            fe16_inst_add(reg, fe16_regs[src>>4]);
        break;
    case sub:
        if(mode)
            fe16_inst_sub(reg, src);
        else
            fe16_inst_sub(reg, fe16_regs[src>>4]);
        break;
    case jmp:
        break;
    case push:
        break;
    case pop:
        break;
    case xor:
        if(mode)
            fe16_inst_xor(reg, src);
        else
            fe16_inst_xor(reg, fe16_regs[src>>4]);
        break;
    case and:
        if(mode)
            fe16_inst_add(reg, src);
        else
            fe16_inst_add(reg, fe16_regs[src>>4]);
        break;
    default:
        fprintf(stderr, "Error");
        exit(1);
    }
    fe16_update_flags(reg);
}

void print_regs()
{
    for(int i = 0; i < reg_count; i++) {
        printf("%d\n", fe16_regs[i]);
    }
}

int main(int argc, char **argv)
{
    FILE *fp;
    uint16_t size, data;

    if(argc < 2) {
        fprintf(stderr, "usage: fe16vm [file]\n");
        exit(1);
    }
    fp = fopen(argv[1], "rb");
    if(fp == NULL) {
        perror(argv[1]);
        exit(1);
    }

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) / 2;
    fseek(fp, 0L, SEEK_SET);

    print_regs();
    puts("--------");
    for(int i = 0; i < size; i++) {
        fread(&data, sizeof(uint16_t), 1, fp);
        fe16_inst_execute(data);
    }
    puts("--------");
    print_regs();
    return 0;
}
