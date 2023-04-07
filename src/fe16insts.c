#include "fe16insts.h"
#include "fe16regs.h"
#include "fe16opcodes.h"
#include "fe16flags.h"
#include "fe16syscalls.h"
#include "fe16memory.h"


void fe16_inst_mov(Fe16_reg reg, uint16_t src)
{
    fe16_reg_write(reg, src);
}

void fe16_inst_add(Fe16_reg reg, uint16_t src)
{
    fe16_reg_write(reg, fe16_reg_read(reg) + src);
}

void fe16_inst_sub(Fe16_reg reg, uint16_t src)
{
    fe16_reg_write(reg, fe16_reg_read(reg) - src);
}

void fe16_inst_jump(uint16_t src)
{
    fe16_reg_write(fpc, 0x2fff + src);
}

void fe16_inst_push(uint16_t src)
{
    uint16_t addr = fe16_reg_read(fsp);
    fe16_mem_write(addr, src);
    fe16_reg_write(fsp, addr+1);
}

void fe16_inst_pop(uint16_t src)
{
    uint16_t addr = fe16_reg_read(fsp);
    uint16_t val = fe16_mem_read(addr);
    fe16_reg_write(src, val);
    fe16_reg_write(fsp, addr - 1);   
}

void fe16_inst_and(Fe16_reg reg, uint16_t src)
{
    uint16_t res = fe16_reg_read(reg) & src;
    fe16_reg_write(reg, res);
}

void fe16_inst_xor(Fe16_reg reg, uint16_t src)
{
    uint16_t res = fe16_reg_read(reg) ^ src;
    fe16_reg_write(reg, res);
}

void fe16_inst_cmp(Fe16_reg reg, uint16_t src)
{
    if(fe16_reg_read(reg) < src) {
        fe16_reg_write(fcd, nf);
    }
    else if(fe16_reg_read(reg) == src) {
        fe16_reg_write(fcd, zf);
    } else {
        fe16_reg_write(fcd, 0);
    }
}

void fe16_inst_jme(uint16_t src)
{
    if(fe16_reg_read(fcd) == zf) {
        fe16_reg_write(fpc, 0x2fff + src);
    }
}

void fe16_inst_sys(void)
{
    switch(fe16_reg_read(f4)) {
    case sys_exit:
        fe16_sys_exit(fe16_reg_read(f0));
        break;
    case sys_write:
        fe16_sys_write(fe16_reg_read(f0), fe16_reg_read(f1));
        break;
    case sys_read:
        fe16_sys_read(fe16_reg_read(f0), fe16_reg_read(f1));
        break;
    }
}
