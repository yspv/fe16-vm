#ifndef FE16SYSCALLS_H
#define FE16SYSCALLS_H
#include <stdint.h>

typedef enum {
    sys_exit = 1,
    sys_write,
    sys_read,
} Fe16_sys_call;

/* functions */
void fe16_sys_exit(uint16_t code);
void fe16_sys_write(uint16_t fd, uint16_t len);
void fe16_sys_read(uint16_t fd, uint16_t len);

#endif
