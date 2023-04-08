#ifndef FE16VM_H
#define FE16VM_H

#include <stdint.h>
#include "fe16opcodes.h"

#define SP_BEGIN 0x1fff
#define PC_START 0x3000

void fe16_execute(uint16_t inst);

static Fe16_opcode fe16_get_opcode(uint16_t inst);

static uint16_t fe16_get_mode(Fe16_opcode op, uint16_t inst);

static uint16_t fe16_get_dreg(Fe16_opcode op, uint16_t inst);

static uint16_t fe16_get_src(uint16_t inst);

#endif
