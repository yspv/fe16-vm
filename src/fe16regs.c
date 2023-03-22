#include "fe16regs.h"
#include <assert.h>

uint16_t fe16_reg_read(Fe16_reg reg)
{
    assert(reg < reg_count);
    return fe16_regs[reg];
}

void fe16_reg_write(Fe16_reg reg, uint16_t data)
{
    assert(reg < reg_count);
    fe16_regs[reg] = data;
}
