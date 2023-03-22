#include "fe16memory.h"
#include <assert.h>
#include <stdio.h>

uint16_t fe16_mem_read(uint16_t addr)
{
    assert(addr > 0x1fff && addr < 0xffff);
    return fe16_memory[addr];
}

uint16_t *fe16_mem_get_addr(uint16_t addr)
{
    assert(addr > 0x1fff && addr < 0xffff);
    return &(fe16_memory[addr]);
}

void fe16_mem_write(uint16_t addr, uint16_t data)
{
   uint16_t *ptr = fe16_mem_get_addr(addr);
   *ptr = data;
}


