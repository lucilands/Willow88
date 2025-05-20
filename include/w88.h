#ifndef _W88_H
#define _W88_H
#include <stdint.h>

#define W88_MEMMAX 64*1024

#define W88_RAM_START 0x0000
#define W88_VRAM_START 0x2000
#define W88_IO_START 0x4000
#define W88_AUDIO_START 0x6000
#define W88_PROG 0x8000

enum {
    W88_REG_A  = 0x00,
    W88_REG_X  = 0x01,
    W88_REG_Y  = 0x02,
    W88_REG_SP = 0x03,
    W88_REG_PC = 0x04,
    W88_REG_SR = 0x05,
    W88_REG_COUNT = 0x06
};

enum {
    W88_OP_NOP    = 0x00,
    
    W88_OP_LDAI   = 0x01,
    W88_OP_LDAA   = 0x02,
    W88_OP_STA    = 0x03,
    W88_OP_LDXI   = 0x04,
    W88_OP_LDXA   = 0x05,
    W88_OP_LDYI   = 0x06,
    W88_OP_LDYA   = 0x07,
    W88_OP_STX    = 0x08,
    W88_OP_STY    = 0x09,
    W88_OP_MOV    = 0x0A,

    W88_OP_ADDI   = 0x10,
    W88_OP_ADDX   = 0x11,
    W88_OP_ADDY   = 0x12,
    W88_OP_SUB    = 0x13,
    W88_OP_AND    = 0x14,
    W88_OP_OR     = 0x15,
    W88_OP_XOR    = 0x16,
    W88_OP_INC    = 0x17,
    W88_OP_DEC    = 0x18,
    W88_OP_SHL    = 0x19,
    W88_OP_SHR    = 0x1A,

    W88_OP_JMP    = 0x20,
    W88_OP_JSR    = 0x21,
    W88_OP_RTS    = 0x22,
    W88_OP_BEQ    = 0x23,
    W88_OP_BNE    = 0x24,
    W88_OP_BCC    = 0x25,
    W88_OP_BCS    = 0x26,
    
    W88_OP_PUSH   = 0x30,
    W88_OP_POP    = 0x31,

    W88_OP_IN     = 0x40,
    W88_OP_OUT    = 0x41,
    W88_OP_WAIT   = 0x42,
    W88_OP_DRAW   = 0x43,
    W88_OP_HLT    = 0x44,
};

enum {
    W88_SRFLAG_NEGATIVE = 0,
    W88_SRFLAG_OVERFLOW,
    W88_SRFLAG_ZERO,
    W88_SRFLAG_CARRY
};

extern uint16_t w88_memory[W88_MEMMAX];
extern uint16_t w88_regs[W88_REG_COUNT];

void W88LoadRom(const char *path);
void W88Memdump(const char *outpath);

void W88GraphicsInit();

#endif //_W88_H