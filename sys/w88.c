#include <w88.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <string.h>


uint16_t w88_memory[W88_MEMMAX] = {0};
uint16_t w88_regs[W88_REG_COUNT] = {0};

void W88LoadRom(const char *path) {
    FILE *cartridge = fopen(path, "rb");
    if (!cartridge) {
        printf("ERROR: Failed to open cartridge %s: %s\n", path, strerror(errno));
        exit(1);
    }

    fseek(cartridge, 0L, SEEK_END);
    unsigned int len = ftell(cartridge) / 2;
    fseek(cartridge, 0L, SEEK_SET);

    fread(w88_memory + W88_PROG, sizeof(uint16_t), len, cartridge);

    fclose(cartridge);
    printf("INFO: Loaded cartridge %s\n", path);
}

void W88Memdump(const char *outpath) {
    FILE *output = fopen(outpath, "w");

    fprintf(output, "w88_regs:\n\tA: 0x%04X\n\tX: 0x%04X\n\tY: 0x%04X\n\tSP: 0x%04X\n\tPC: 0x%04X\n", w88_regs[W88_REG_A], w88_regs[W88_REG_X], w88_regs[W88_REG_Y], w88_regs[W88_REG_SP], w88_regs[W88_REG_PC]);

    for (unsigned int i = 0; i < W88_MEMMAX; i++) {
        switch (i) {
            case W88_RAM_START: fprintf(output, "\nw88_ram:"); break;
            case W88_VRAM_START: fprintf(output, "\nw88_vram:"); break;
            case W88_IO_START: fprintf(output, "\nw88_io_regs:"); break;
            case W88_AUDIO_START: fprintf(output, "\nw88_audio_regs:"); break;
            case W88_PROG: fprintf(output, "\nw88_cartridge:"); break;
            
            default: break;
        }
        if (i % 8 == 0) fprintf(output, "\n\t0x%04X:", i);
        fprintf(output, " 0x%04X", w88_memory[i]);
    }

    fclose(output);
    printf("INFO: Generated memdump in %s\n", outpath);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: Invalid usage\n");
        printf("%s [cartridge.w88]\n", argv[0]);
        return 1;
    }
    W88LoadRom(argv[1]);

    bool hlt = false;

    uint16_t inst = 0;
    uint8_t opcode = 0;
    uint8_t operand = 0;

    w88_regs[W88_REG_PC] = W88_PROG;
    while (!hlt) {
        inst = w88_memory[w88_regs[W88_REG_PC]];
        opcode = (inst >> 8) & 0xFF;
        operand = inst & 0xFF;

        switch (opcode) {
            case W88_OP_NOP: break;
            case W88_OP_HLT: hlt = true; break;

            case W88_OP_LDAA: w88_regs[W88_REG_A] = w88_memory[operand]; break;
            case W88_OP_LDXA: w88_regs[W88_REG_X] = w88_memory[operand]; break;
            case W88_OP_LDYA: w88_regs[W88_REG_Y] = w88_memory[operand]; break;

            case W88_OP_LDAI: w88_regs[W88_REG_A] = operand; break;
            case W88_OP_LDXI: w88_regs[W88_REG_X] = operand; break;
            case W88_OP_LDYI: w88_regs[W88_REG_Y] = operand; break;

            case W88_OP_STA: w88_memory[operand] = w88_regs[W88_REG_A]; break;
            case W88_OP_STX: w88_memory[operand] = w88_regs[W88_REG_X]; break;
            case W88_OP_STY: w88_memory[operand] = w88_regs[W88_REG_Y]; break;

            case W88_OP_JMP: w88_regs[W88_REG_PC] = operand; break;
            

            default: {
                // BAD OPCODE
                printf("WARNING: Bad opcode 0x%02X\n", opcode);
                break;
            }
        }
        w88_regs[W88_REG_PC]+=1;
    }
    W88Memdump("W88memdump.txt");
    return 0;
}