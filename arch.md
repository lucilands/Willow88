# Willow88 Architecture spec


## Memory
The Willow88 will have 64KB of memory space, allocated as follows

### Registers
Willow88 has 6 16-bit registers.
 - `A`: Accumulator register (Main register for math and logic)
 - `X`, `Y`: Index registers
 - `SP`: Stack pointer
 - `PC`: Program counter
 - `SR`: Status register

### Memmap
| Adress Range | Purpose |
| ------------ | ------- |
| 0000 - 1FFF | 8KB RAM |
| 2000 - 3FFF | 8KB VRAM |
| 4000 - 5FFF | I/O Regs |
| 6000 - 7FFF | Audio RAM/Regs |
| 8000 - FFFF | ROM (Cartrige) |

## Graphics subsystem
 - Sports a 256 by 192 pixel screen
 - Supports 8 by 8 pixel, tilemap-based tiles 
 - Up to 64 sprites on screen, either 8x8 or 8x16
 - And 16 colors in total, 4 per sprite/tile 

### VRAM Layout
 - Tilemap (32x24 tiles = 768 bytes)
 - Tile set (256 tiles x 16 bytes = 4KB)
 - Sprite attribute table (position, index, pallete) (Variable size)

# Instruction set 
Instructions in Willow88 are stored as a 3 byte value, no matter the instruction, however parameters that aren't used are ignored and should be set to 0x00.

## Data movement
| Mnemonic | Opcode | Format | Description |
| -------- | ------ | ------ | ----------- |
| LDA imm | 0x01 | LDA 0x34 | Load immediate into A |
| LDA addr | 0x02 | LDA $1234 | Load A from memory |
| STA addr | 0x03 | STA $1234 | Store A to memory|
| LDX imm | 0x04 | LDX 0x34 | Load immediate into X |
| LDY imm | 0x05 | LDY 0x34 | Load immediate into Y |
| LDX addr | 0x06 | LDX $1234 | Load X from memory |
| LDY addr | 0x07 | LDY $1234 | Load Y from memory |
| STX addr | 0x08 | STX $1234 | Store X to memory |
| STY addr | 0x09 | STY $1234 | Store Y to memory |
| MOV A, X | 0x0A | - | Copy X into A |
| MOV X, A | 0x0B | - | Copy A into X |
| MOV A, Y | 0x0C | - | Copy Y into A |
| MOV Y, A | 0x0D | - | Copy A into Y |

## Arithmetic and Logic
| Mnemonic | Opcode | Format | Description |
| -------- | ------ | ------ | ----------- |
| ADD A, imm | 0x10 | ADD A, 0x34 | Add immediate to A |
| ADD A, B | 0x11 | - | Add B to A |
| SUB A, imm | 0x12 | ADD A, 0x34 | Subtract immediate to A|
| AND A, imm | 0x13 | AND A, 0x34 | Logical AND |
| OR A, imm | 0x14 | OR A, 0x34 | Logical OR |
| XOR A, imm | 0x15 | XOR A, 0x34 | Logical XOR |
| INC A | 0x16 | - | Increment A |
| DEC A | 0x17 | - | Decrement A |
| SHL A | 0x18 | - | Logical shift left |
| SHR A | 0x19 | - | Logical shift right |

## Control flow 
| Mnemonic | Opcode | Format | Description |
| -------- | ------ | ------ | ----------- |
| JMP addr | 0x20 | JMP $1234 | Unconditional jump |
| JSR addr | 0x21 | JSR $1234 | Jump to subroutine |
| RTS | 0x22 | - | Return from subroutine |
| BEQ addr | 0x23 | BEQ $1234 | Branch if zero flag is set |
| BNE addr | 0x24 | BNE $1234 | Branch if zero flag not set |
| BCC addr | 0x25 | BCC $1234 | Branch if carry flag set |
| BCS addr | 0x26 | BCS $1234 | Branch if carry flag not set |

## Stack operations
| Mnemonic | Opcode | Format | Description |
| -------- | ------ | ------ | ----------- |
| PUSH A | 0x30 | - | Push A to stack |
| POP A | 0x31 | - | Pop from stack into A |
| PUSH SR | 0x32 | - | Push status flags |
| POP SR  | 0x33 | - | Pop into status flags |

## System specific
| Mnemonic | Opcode | Format | Description |
| -------- | ------ | ------ | ----------- |
| IN #port | 0x40 | IN #1 | Read byte from port |
| OUT #port | 0x41 | OUT #1 | Write byte to port |
| WAIT | 0x42 | - | Wait one frame |
| DRAW | 0x43 | - | Trigger redraw |
| HLT | 0x44 | - | Stop |
| NOP | 0x00 | NOP | Do nothing |
