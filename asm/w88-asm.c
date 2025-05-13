#include <stdio.h>
#include <stdbool.h>

#include <w88.h>
#include <w88-asm.h>

#include <errno.h>
#include <string.h>

#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

char *trim(char *s) {
    char *end;
    while (*s && (isspace((int)*s))) s++;
    if (!*s) return s;
    end = s + strlen(s) - 1;
    while (end > s && (isspace((int)*end))) end--;
    end[1] = '\0';
    return s;
}

char **split(char *s, char delim, int *count) {
    static char *out[64];
    int i = 0;
    while (*s) {
        while (*s == delim) s++;  // Skip delimiters
        if (!*s) break;  // If at the end of the string, stop
        out[i++] = trim(s);  // Record the start of a token
        while (*s && *s != delim) s++;  // Skip the token
        if (*s) *s++ = 0;  // Null-terminate the token and move past delimiter
    }
    *count = i;
    return out;
}

uint16_t addr_imm_inst(char **parts, char *name, char *file, int lineno, int num, uint8_t opcode_addr, uint8_t opcode_imm) {
    uint8_t opcode = 0;
    uint8_t operand = 0;
    if (num < 2) {
        printf("ERROR %s:%i: instruction %s expects an operand\n", file, lineno, name);
        exit(1);
    }
    if (parts[1][0] == '$') {
        opcode = opcode_addr;
        operand = atoi(parts[1]+1);
    }
    else {
        opcode = opcode_imm;
        operand = strtol(parts[1], NULL, 16);
    }
    return ((uint16_t)opcode << 8) | operand;
}

uint16_t addr_inst(char **parts, char *name, char *file, int lineno, int num, uint8_t opcode, uint8_t _) {
    uint8_t operand = 0;
    (void)_;
    if (num < 2) {
        printf("ERROR %s:%i: instruction %s expects an operand\n", file, lineno, name);
        exit(1);
    }
    if (parts[1][0] == '$') {
        operand = atoi(parts[1]+1);
    }
    else {
        printf("ERROR %s:%i: instruction %s expects address operand\n", file, lineno, name);
        exit(1);
    }

    return ((uint16_t)opcode << 8) | operand;
}

uint16_t nop_inst(char **parts, char *name, char *file, int lineno, int num, uint8_t opcode, uint8_t _) {
    (void)parts;
    (void)_;
    if (num > 1) {
        printf("ERROR %s:%i: instruction %s requires no arguments", file, lineno, name);
        exit(1);
    }
    return ((uint16_t)opcode << 8) | 0;
}

const w88_asm_instruction_t instructions[] = {
    {"LDA", addr_imm_inst, W88_OP_LDAA, W88_OP_LDAI},
    {"LDX", addr_imm_inst, W88_OP_LDXA, W88_OP_LDXI},
    {"LDY", addr_imm_inst, W88_OP_LDYA, W88_OP_LDYI},

    {"STA", addr_inst, W88_OP_STA, 0},
    {"STX", addr_inst, W88_OP_STX, 0},
    {"STY", addr_inst, W88_OP_STY, 0},

    {"JMP", addr_inst, W88_OP_JMP, 0},
    {"JSR", addr_inst, W88_OP_JSR, 0},
    {"BEQ", addr_inst, W88_OP_BEQ, 0},
    {"BNE", addr_inst, W88_OP_BNE, 0},
    {"BCC", addr_inst, W88_OP_BCC, 0},
    {"BCS", addr_inst, W88_OP_BCS, 0},

    {"NOP", nop_inst, W88_OP_NOP, 0},
    {"HLT", nop_inst, W88_OP_HLT, 0},
    {"RTS", nop_inst, W88_OP_RTS, 0},
    {"WAIT", nop_inst, W88_OP_WAIT, 0},
    {"DRAW", nop_inst, W88_OP_DRAW, 0},
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s [input.wa] ...\n", argv[0]);
        return 1;
    }

    char *file = malloc(strlen(argv[1])+1);
    strcpy(file, argv[1]);

    FILE *input = fopen(file, "r");
    if (!input) {printf("ERROR: Failed to open file %s: %s\n", file, strerror(errno));}
    int c;
    char outname[MAX_LINE_LENGTH] = {0};
    strcat(outname, split(argv[1], '.', &c)[0]);
    strcat(outname, ".w88");
    FILE *output = fopen(outname, "wb");

    char line[MAX_LINE_LENGTH];
    int lineno = 1;
    int num = 0;
    uint16_t inst = 0;
    char **parts = NULL;
    while (fgets(line, sizeof(line), input)) {
        if (strlen(trim(line)) < 2) {lineno++; continue;}
        inst = 0;
        parts = split(trim(line), ' ', &num);
        
        bool invalid_op = false;
        
        for (unsigned int i = 0; i < sizeof(instructions) / sizeof(w88_asm_instruction_t); i++) {
            invalid_op = false;
            if (strcmp(parts[0], instructions[i].name) == 0) {
                inst = instructions[i].instfunc(parts, instructions[i].name, file, lineno, num, instructions[i].inst_a, instructions[i].inst_b);
                break;
            }
            invalid_op = true;
        }
        
        if (invalid_op) {
            printf("ERROR %s:%i: Invalid operation %s\n", file, lineno, parts[0]);
        }
        
        fwrite(&inst, sizeof(inst), 1, output);
        lineno++;
    }
    
    fclose(input);
    fclose(output);
    return 0;
}