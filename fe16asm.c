#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fe16vm.h"
#define INST_LEN 80

static char *fe16_opcodes[op_count] = {
    "mov", "add", "sub", "jmp",
    "push", "pop", "xor",
    "and",
};

static char *fe16_regs[reg_count] = {
    "f0", "f1", "f2", 
    "f3", "f4", "f5"
};


static uint16_t fe16asm_generate_opcode(const char *strln)
{
    uint16_t op = 0;
    for(op = 0; op < op_count; op++) {
        if(strstr(strln, fe16_opcodes[op]) != NULL) {
            return op << 12;
        }
    }
    return op_count + 1;
}

static uint16_t fe16asm_generate_dreg(const char *strln)
{
    uint16_t dreg = 0;
    for(dreg = 0; dreg < reg_count; dreg++) {
        if(strstr(strln, fe16_regs[dreg]) != NULL) {
            return dreg << 9;
        }
    }
    return reg_count + 1;
}

static uint16_t fe16asm_generate_imm(char *strln)
{
    uint16_t imm = 0;
    for(int i = 0; strln[i] != '\0'; i++) {
        if((strln[i] > 057) && (072 > strln[i])) {
            if(strln[i - 1] != 'f') {
                imm = atoi(&(strln[i]));
                return imm + (1 << 8);
            }

        }
    }
    return 1 << 9;
}

static uint16_t fe16asm_generate_reg(char *strln)
{
    char *src = NULL;
    for(int i = 0; i < reg_count; i++) {
        src = strstr(strln, fe16_regs[i]);
        if(src != NULL)
            break;
    }
    for(int l = 0; src && (l < reg_count); l++) {
        if(strstr(src, fe16_regs[l]) != NULL) {
            return l << 4;
        }
    }
    return op_count + 1;
}

static void fe16asm_compile(char *strln)
{
       
}

int main(int argc, char **argv)
{
    int fd;
    char *map = NULL;
    uint16_t op, reg, src;
    struct stat st;
    if(argc < 2) {
        fprintf(stderr, "usage: fe16asm [filename]\n");
        exit(1);
    }
    fd = open(argv[1], O_RDWR, 0600);
    if(fd == -1) {
        perror(argv[1]);
        exit(2);
    }
    if(fstat(fd, &st) == -1) {
        perror("fstat");
        exit(2);
    }
    if(st.st_size == 0) {
        fprintf(stderr, "Error: File is empty");
        exit(2);
    }
    map = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(map == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    unsigned int l, flag;
    flag = l = 0;
    while(1) {
        char *tmp;
        int i = 0;
        if(l == st.st_size) {
            break;
        }
        tmp = malloc(INST_LEN);
        for(; l < st.st_size; l++) {
            if(!flag & (map[l] == ';')) {
                flag = 1;       /* start comments */
                continue;
            }
            else if(flag & (map[l] == '\n')) {
                flag = 0;      /* end of comments */
                l -= 1;
                continue;
            }
            else if(!flag & (map[l] != '\n' )) {
                tmp[i] = map[l];
                i++;
            } 
            else if(!flag){
                tmp[i] = '\0';
                op = fe16asm_get_opcode(tmp);
                reg = fe16asm_get_dreg(tmp);
                src = fe16asm_get_reg(tmp);
                printf("%d %d %d\n", op, reg, src);
                l += 1;
                break;
            }
        } /* end of for */
        free(tmp);
    } /* end of loop */

    return 0;
}
