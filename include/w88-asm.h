#ifndef _W88_ASM_H
#define _W88_ASM_H

#include <stdint.h>

typedef struct {
    char *name;
    uint16_t (*instfunc)(char **, char *, char *, int, int, uint8_t, uint8_t);
    uint8_t inst_a;
    uint8_t inst_b;
} w88_asm_instruction_t;

typedef struct {
    char *name;
    uint8_t reg_id;
} w88_asm_reg_t;

#endif //_W88_ASM_H