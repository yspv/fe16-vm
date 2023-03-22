#ifndef _FE16MEMORY_H
#define _FE16MEMORY_H

#include <stdint.h>

#define FE16_MEM_SIZE 1 << 16

typedef enum {
    FE16_MEM_OK = 0,
    FE16_MEM_BAD_INDEX,
    FE16_MEM_ACCESS_DENIED,
} Fe16_mem_error;

static uint16_t fe16_memory[FE16_MEM_SIZE];

uint16_t fe16_mem_read(uint16_t addr);
uint16_t *fe16_mem_get_addr(uint16_t addr);
void fe16_mem_write(uint16_t addr, uint16_t data);

#endif
