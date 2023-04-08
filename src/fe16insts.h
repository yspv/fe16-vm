#ifndef FE16_INSTS_H
#define FE16_INSTS_H

#include <stdint.h>
#include "fe16regs.h"

void fe16_inst_mov(Fe16_reg reg, uint16_t src);
void fe16_inst_add(Fe16_reg reg, uint16_t src);
void fe16_inst_sub(Fe16_reg reg, uint16_t src);
void fe16_inst_jump(uint16_t src);
void fe16_inst_push(uint16_t src);
void fe16_inst_pop(Fe16_reg src);
void fe16_inst_and(Fe16_reg reg, uint16_t src);
void fe16_inst_xor(Fe16_reg reg, uint16_t src);
void fe16_inst_cmp(Fe16_reg reg, uint16_t src);
void fe16_inst_jme(uint16_t src);
void fe16_inst_sys(void);
void fe16_inst_call(uint16_t src);
void fe16_inst_ret(void);

#endif
