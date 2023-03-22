#ifndef FE16_REGS_H
#define FE16_REGS_H

#include <stdint.h>

typedef enum {
    f0 = 0,       /* 00 */
    f1,           /* 01 */
    f2,           /* 02  */
    f3,           /* 03  */
    f4,           /* 04  */
    fsp,          /* 05 stack pointer   */
    fcd,          /* 06 conditional     */
    fpc,          /* 07 program counter */
    reg_count,
} Fe16_reg;

typedef enum {
    FE16_REG_OK = 0,
    FE16_REG_WRONG,
    FE16_REG_BAD_DATA,
} Fe16_reg_error;

static uint16_t fe16_regs[reg_count];

uint16_t fe16_reg_read(Fe16_reg reg);
void fe16_reg_write(Fe16_reg reg, uint16_t data);

#endif
