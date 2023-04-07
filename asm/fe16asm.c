#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include "src/fe16opcodes.h"
#include "src/fe16regs.h"
#include <limits.h>

#define ERR_COLOR "\033[0;36m"

char *fe16_opcode_arr[op_count] = {
    "mov",  "add", "sub", "jmp",
    "push", "pop", "xor", "and",
    "sys",  "cmp", "jme"
};

char *fe16_reg_arr[reg_count] = {
    "f0", "f1", "f2", "f3", "f4",
    "fsp", "fcd", "fpc"
};

int fe16asm_get_line_count(char *buf)
{
    int count = 0;
    for(int i = 0; buf[i] != '\0'; i++) {
        if(buf[i] == '\n')
            count++;
    }
    return count;
}

int fe16asm_get_line_length(const char *buf)
{
    int i;
    for(i = 0; buf[i] != '\n' && buf[i] != '\0'; i++)
    {
        ;
    }
    return i;
}

int fe16asm_is_later(char ch)
{
    return ch >= 'A' && ch <= 'z'? 1: 0;
}

int fe16asm_is_number(char ch)
{
    return ch >= '0' && ch <= '9'? 1: 0;
}

char *fe16asm_get_line(const char *buf, int len)
{
    
    int i;
    int l = 0;
    int flag = 0;
    char *line = malloc(len);
    if(line == NULL)
        return NULL;
    for(i = 0; i < len; i++) {
        if(fe16asm_is_later(buf[i]) || fe16asm_is_number(buf[i])) {
            flag = 1;
            line[l] = buf[i];
            l++;
        }
        else if(flag && buf[i] == ' ') {
            flag = 0;
            line[l] = ' ';
            l++;
        }
        else if(buf[i] == '\n')
            break;
    }
    line[l] = '\0';
    return line;
}

int fe16asm_get_code_count(const char *line, int size)
{
    int count = 0;
    int flag = 0;
    for(int i = 0; i < size; i++) {
        if(flag && line[i] == ' ') {
            flag = 0;
        }
        else if(!flag && (fe16asm_is_later(line[i]) || 
                fe16asm_is_number(line[i]))) {
            flag = 1;
            count++;
        }
    }

    return count;
}

char **fe16asm_parse_line(const char *line, int len, int cc)
{ 
    char **inst_arr = malloc(sizeof(char *) * (cc + 1));
    inst_arr[cc] = NULL;
    for(int s = 0; s < cc ; s++) {
        char *tmp;
        int count, c;

        count = 0;
        while(line[count] != ' ' && line[count] != '\0') {
            count += 1;
        }

        if(count == 0) {
            s -= 1;
            line += 1;
            continue;
        }

        tmp = malloc(count + 1);
        if(tmp == NULL) {
            return NULL;
        }
        tmp[count] = '\0';
        
        c = 0;
        for(int l = 0; l < count; l++) {
            if(fe16asm_is_later(line[l]) || 
                    fe16asm_is_number(line[l])) {
                tmp[c] = line[l];
                c += 1;
            }
        }
        inst_arr[s] = tmp;
        line += count;
    }
   
    return inst_arr;
}


void fe16asm_free_inst_arr(char **inst_arr)
{
    while(*inst_arr != NULL) {
        free(*inst_arr);
        inst_arr++;
    }
}

int fe16asm_get_opcode(char **inst_arr)
{
    for(int i = 0; i < op_count; i++)
        if(strcmp(inst_arr[0], fe16_opcode_arr[i]) == 0)
            return i << 12;

    return -1;
}

int fe16asm_get_dreg(char **inst_arr)
{
    for(int i = 0; i < reg_count; i++)
        if(strcmp(inst_arr[1], fe16_reg_arr[i]) == 0) 
            return i << 9;

    return -1;
}

int fe16asm_int_degree(int num, int deg)
{
    int res = 1;
    for(int i = 0; i < deg - 1; i++) {
        res *= num;
    }
    return res;
}

int fe16_str_int(const char *str, int *var)
{
/*
 *  0 in ascii 48
 *  9 in ascii 57
 */
    int num = 0;
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        if(str[i] > 47 && str[i] < 58) 
            num += (str[i] - 48) * fe16asm_int_degree(10, len - i);
        else
            return -1;
    }
    *var = num;
    return 0;
}

int fe16asm_get_src(char **inst_arr, int flag)
{
/*
 *  if flag equal to 1 then opcode does not need destination reg
 *  exp:
 *      push f3
 *
 */
    int src, ret;
    if(!flag) {
        ret = fe16_str_int(inst_arr[2], &src);
        if(ret == -1) {
            for(int i = 0; i < reg_count; i++)
                if(strcmp(inst_arr[2], fe16_reg_arr[i]) == 0)
                    return i << 4;

            return -1;
        }
        return src + (1 << 8);
    } else {
        ret = fe16_str_int(inst_arr[1], &src);
        if(ret == -1) {
            for(int i = 0; i < reg_count; i++)
                if(strcmp(inst_arr[1], fe16_reg_arr[i]) == 0)
                    return i << 4;
            return -1;
        }
        return src + (1 << 8);
    }
}

int main(int argc, char **argv)
{
    int source_file, linecount;
    char *map, *buf, *bufptr;
    FILE *fp;
    struct stat st;

    if(argc < 2) {
        fprintf(stderr, "usage: [filename]\n");
        exit(1);
    }
    
    source_file = open(argv[1], O_RDONLY);

    if(source_file ==  -1) {
        perror(argv[1]);
        exit(1);
    }
    if(fstat(source_file, &st) == -1) {
        perror(argv[1]);
        exit(1);
    }

    if(st.st_size == 0) {
        fprintf(stderr, "Error: File is empty!\n");
        exit(1);
    }
    
    fp = fopen("test", "wb");
    if(fp == NULL) {
        perror("test");
        exit(1);
    }

    buf = (char *)malloc(st.st_size);
    map = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, source_file, 0);

    if(map == MAP_FAILED) {
        perror(argv[1]);
        exit(1);
    }
    
    int l = 0;    /* buf index    */
    int flag = 0; /* comment flag */
    for(int i = 0; i < st.st_size; i++) {
        if(map[i] == ';') {
            flag = 1;   /* begin comment */
        }
        else if(flag && map[i] == '\n') {
            flag = 0;   /* end of comment */
        }
        if(!flag) {
            buf[l] = map[i];
            l++;
        }
    } /* end of for() */

    /* generate code */
    bufptr = buf;
    linecount = fe16asm_get_line_count(buf);
    for(int i = 0; i < linecount; i++) {
        long linelen, count, op, dreg, src, flag;
        uint16_t inst;
        char *line, **arr;

        linelen = fe16asm_get_line_length(bufptr);
        if(linelen == 0) {
            goto next_line;
        }

        line = fe16asm_get_line(bufptr, linelen);
        if(line == NULL) {
            perror("malloc");
            exit(2);
        }

        count = fe16asm_get_code_count(line, linelen);

        arr = fe16asm_parse_line(line, linelen, count);

        if(arr == NULL) {
            perror("malloc");
            exit(2);
        }

        op    = fe16asm_get_opcode(arr);
        if(op < 0) {
            int col = strstr(line, arr[0]) - line;

            fprintf(stderr, "%s%s:%d:%d: error: wrong opcode '%s'\n",
                    ERR_COLOR, argv[1], linecount, col, arr[0]);

            exit(3);
        }
        
        switch(count) {
            case 1:
                dreg = 0;
                src  = 0;
                goto gen_inst;
                break;
            case 2:
                dreg = 0;
                goto get_src;
                break;
        }

        dreg  = fe16asm_get_dreg(arr);

        if(dreg < 0) {
            int col = strstr(line, arr[1]) - line;

            fprintf(stderr, "%s%s:%d:%d: error: wrong destination " \
                    "register '%s'\n", ERR_COLOR, argv[1],
                    linecount, col, arr[1]);

            exit(3);
        }
get_src:
        flag = count > 2 ? 0 : 1;
        src = fe16asm_get_src(arr, flag);

        if(src < 0) {
            char *str = flag ? arr[1] : arr[2];

            int col = flag ? 
                        strstr(line, arr[1]) - line :
                        strstr(line, arr[2]) - line;

            fprintf(stderr, "%s%s:%d:%d: error: wrong source '%s'\n",
                    ERR_COLOR, argv[1], linecount, col, str);

            exit(3);
        }
gen_inst:

        inst = op + dreg + src;
        fwrite(&inst, sizeof(uint16_t), 1, fp);

        fe16asm_free_inst_arr(arr);
        free(arr);
next_line:
        bufptr += linelen + 1;
    }/* end of for() */

    munmap(map, st.st_size);
    free(buf);
    return 0;
}
