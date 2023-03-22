/*
 * ____________***________
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
 */
#include "fe16vm.h"
#include "fe16regs.h"
#include "fe16insts.h"
#include "fe16memory.h"
#include "fe16opcodes.h"
#include "fe16syscalls.h"
#include "fe16flags.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>


static Fe16_opcode fe16_get_opcode(uint16_t inst)
{
    return inst >> 12;
}

static uint16_t fe16_get_mode(Fe16_opcode op, uint16_t inst)
{
    uint16_t imm = ((op << 12) ^ inst);
    if(imm > (imm ^ 0x100)) {
        return 1;                          /* Immadiate mode */
    }
    return 0;                              /* Register mode */
}

/* For get destention registor */
static uint16_t fe16_get_dreg(Fe16_opcode op, uint16_t inst)
{
    return ((op << 12) ^ inst) >> 9;
}

static uint16_t fe16_get_src(uint16_t inst)
{
    return ((inst >> 8) << 8) ^ inst;
}

static void fe16_update_flags(Fe16_reg reg)
{
    if(fe16_reg_read(reg) == 0) {
        fe16_reg_write(fcd, zf);
    } 
    else if(fe16_reg_read(reg) >> 15) {
        fe16_reg_write(fcd, nf);
    } else {
        fe16_reg_write(fcd, 0);
    }
}

void fe16_execute(uint16_t inst)
{
    Fe16_opcode op = fe16_get_opcode(inst);
    uint16_t dreg = fe16_get_dreg(op, inst);
    uint16_t mode = fe16_get_mode(op, inst);
    uint16_t src = fe16_get_src(inst);
    
    switch(op) {
    case mov:
        if(mode) {
            fe16_inst_mov(dreg, src);
        } else {
            fe16_inst_mov(dreg, fe16_reg_read(src>>4));
        }
        break;
    case add:
        if(mode) {
            fe16_inst_add(dreg, src);
        } else {
            fe16_inst_mov(dreg, fe16_reg_read(src>>4));
        }
        break;
    case sub:
        if(mode) {
            fe16_inst_sub(dreg, src);
        } else {
            fe16_inst_sub(dreg, fe16_reg_read(src>>4));
        }
        break;
    case jmp:
        if(mode) {
            fe16_inst_sub(dreg, src);
        } else {
            fe16_inst_sub(dreg, fe16_reg_read(src>>4));
        }
        break;
    case push:
        if(mode) {
            fe16_inst_push(src);
        } else {
            fe16_inst_push(fe16_reg_read(src>>4));
        }
        break;
    case pop:
        if(mode) {
            fe16_inst_pop(src);
        } else {
            fe16_inst_pop(fe16_reg_read(src>>4));
        }
        break;
    case xor:
        if(mode) {
            fe16_inst_xor(dreg, src);
        } else {
            fe16_inst_xor(dreg, fe16_reg_read(src>>4));
        }
        break;
    case and:
        if(mode) {
            fe16_inst_and(dreg, src);
        } else {
            fe16_inst_and(dreg, fe16_reg_read(src>>4));
        }
        break;
    case sys:
        fe16_inst_sys();
        break;
    case cmp:
        if(mode) {
            fe16_inst_cmp(dreg, src);
        } else {
            fe16_inst_cmp(dreg, fe16_reg_read(src>>4));
        }
        return;
    case jme:
        if(mode) {
            fe16_inst_jme(src);
        } else {
            fe16_inst_jme(fe16_reg_read(src>>4));
        }
        break;
    default:
        exit(10);
    }
}


static void print_regs()
{
    for(int i = 0; i < reg_count; i++) {
        printf("%d\n", fe16_reg_read(i));
    }
}

uint16_t program[] = {
    0b0100000101101000,
    0b0100000101100101,
    0b0100000101101100,
    0b0100000101101100,
    0b0100000101101111,
    0b0000000100000001,
    0b0000001100000101,
    0b0000100100000010,
    0b1000000000000000,
    0b0110000000000000,
    0b0000100100000001,
    0b1000000000000000,
};

uint16_t program1[] = {
    0b0100000101101000,
    0b0100000101100101,
    0b0100000101101100,
    0b0100000101101100,
    0b0100000101101111,
    0b0100000100000000,
    0b0000000100000001,
    0b0000001100000110,
    0b0000100100000010,
    0b1000000000000000,
    0b0000000100000000,
    0b0000100100000001,
    0b1000000000000000,
};

int main(int argc, char **argv)
{
    FILE *fp;
    uint16_t size;

    if(argc < 2) {
        fprintf(stderr, "usage: fe16vm [file]\n");
        exit(1);
    }
    fp = fopen(argv[1], "rb");
    if(fp == NULL) {
        perror(argv[1]);
        exit(1);
    }

    fe16_reg_write(fsp, 0x2000);
    fe16_reg_write(fpc, 0x3000);

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);             
    fseek(fp, 0L, SEEK_SET);
    
    fread(fe16_mem_get_addr(0x3000), size, 1, fp);

    while(1) {
        uint16_t addr = fe16_reg_read(fpc);
        uint16_t inst = fe16_mem_read(addr);
        fe16_execute(inst);
        fe16_reg_write(fpc, addr + 1);
    }

    return 0;
}
