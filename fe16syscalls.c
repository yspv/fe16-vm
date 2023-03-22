#include "fe16syscalls.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "fe16vm.h"

void fe16_sys_exit(uint16_t code)
{
    assert(code < 256);
    _exit(code);
}

void fe16_sys_write(uint16_t fd, uint16_t len)
{
    uint16_t addr = fe16_regs[fsp] - len;
    assert(addr > 0x1fff);
    write(fd, &(fe16_memory[addr]), len*2);   
}

void fe16_sys_read(uint16_t fd, uint16_t len)
{   
    uint16_t addr = fe16_regs[fsp] + 1;
    assert(addr > 0x1fff);
    read(fd, &(fe16_memory[addr]), len);
    fe16_regs[fsp] += len;
}
