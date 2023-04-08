/*
 * 15____________***______0
 * | PPPP DDD M NNNN NNNN |
 * --||||-|||-|-||||-||||-
 *   |||| ||| | NNNN NNNN - Store a value. 
 *   |||| ||| | 
 *   |||| ||| M - MODE bit. If mode bit 1 other N bits store a number else
 *   |||| |||                 (*) 3 N bits store register.
 *   |||| |||
 *   |||| DDD - Destination bits. Store a destention register address.
 *   ||||
 *   PPPP - Opcode bits.
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
            fe16_inst_add(dreg, fe16_reg_read(src>>4));
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
            fe16_inst_jump(src);
        } else {
            fe16_inst_jump(fe16_reg_read(src>>4));
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
        assert(mode == 0);
        if(!mode) {
            fe16_inst_pop(src>>4);
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
    case call:
        if(mode) {
            fe16_inst_call(src);
        } else {
            fe16_inst_call(fe16_reg_read(src>>4));
        }
        break;
    case ret:
        fe16_inst_ret();
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

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);             
    fseek(fp, 0L, SEEK_SET);
    
    if(!size) {
        fprintf(stderr, "Error: Executed file is empty\n");
        exit(1);
    }

    fe16_reg_write(fsp, SP_BEGIN);
    fe16_reg_write(fpc, PC_START);
    
    fread(fe16_mem_get_addr(PC_START), size, 1, fp);
    
    while(1) {
        uint16_t addr = fe16_reg_read(fpc);
        uint16_t inst = fe16_mem_read(addr);
        fe16_reg_write(fpc, addr + 1);
        fe16_execute(inst);
    }

    return 0;
}
