#ifndef CPU_H
#define CPU_H

#include <tuple>
#include <array>
#include <cstdint>

#include "./opcode.h"

namespace chip
{   
    /**
     *  Representation of the Chip-8 CPU and memory. Chip-8 has
     *  16 general purpose registers but, the VF register can't 
     *  be used by any program.
     * 
     *  Chip-8 has 4Kb of RAM memory. The first 512 bytes, 
     *  from 0x000 to 0x1FF, are where the original interpreter 
     *  was located, and should not be used by programs. Most 
     *  Chip-8 programs start at location 0x200 (512), but some 
     *  begin at 0x600 (1536). Programs beginning at 0x600 are 
     *  intended for the ETI 660 computer. (Cowgods, 1997)
     * 
     *  Also, Chip-8 has a 16 16-bit stack which is managed 
     *  by the SP pointer. Next, Chip-8 has no interrupts
     *  or hardware registers but it has two timers that count
     *  down to zero at 60Hz.
     * 
     *  Finally, although screen and memory are not part of the
     *  CPU perse we are putting it on the struct for convenience. 
     */ 
    struct CPU
    {
        uint8_t  DT;    // Delay Time register.
        uint8_t  ST;    // Sound Time register.    
        uint8_t  SP;    // Stack Pointer.
        uint16_t PC;    // Program Counter.
        uint16_t I;     // Special register I.
        std::array<uint8_t, 16> V; // General purpose registers (Vx).
        std::array<uint8_t, 4096> memory; // memory of the program.
        std::array<uint8_t, 64 * 32> screen; // Chip-8 expects a screen of 64 by 32 pixels.
        std::array<uint16_t,16> stack; // stack for function call.
    };

    static inline void op_code_0xE0(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xEE(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x1(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x2(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x3(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x4(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x50(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x6(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x7(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x80(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x81(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x82(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x83(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x84(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x85(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x86(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x87(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x8E(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0x90(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xA(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xB(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xC(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xD(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xE9E(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xEA1(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF07(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF0A(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF15(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF18(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF1E(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF29(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF33(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF55(CPU& cpu, const OpCode op_code)
    {

    }

    static inline void op_code_0xF65(CPU& cpu, const OpCode op_code)
    {

    }


    /**
     *  Given a Chip-8 binary program and a pointer to the next OpCode, determine
     *  which one of the OpCodes is and return it with its corresponding data.
     *  
     *  However, in some of the instructions like 8XY0 the OpCode is made up
     *  by the first nibble (8) and the last nibble(0) so, all codes that 
     *  have a similar pattern or have the data in the middle 8 bits, the 
     *  function returns a translation like so 8XY0 -> 80 or FX15 -> F15.
     *  
     *  Now, before presenting the OpCodes there is some nomenclature to take
     *  into account:
     * 
     *  X   = id of a register (0 - F).
     *  Y   = id of a register (0 - F).
     *  VX  = register X.
     *  VY  = register Y.
     *  N   = a nibble (4 bits)   
     *  NN  = two nibbles or one byte (8 bits).
     *  NNN = three niblles (12 bits). 
     * 
     *  The possible OpCodes:
     *  
     *  0x00E0 -> 0xE0 = Clears the screen.
     *  0x00EE -> 0xEE = Return from subroutine.
     *  0x1NNN -> 0x1  = Jumps to address NNN.
     *  0x2NNN -> 0x2  = Calls subroutine at NNN.
     *  0x3XNN -> 0x3  = Skips the next instruction if VX equals NN.
     *  0x4XNN -> 0x4  = Skips the next instruction if VX doesn't equal NN
     *  0x5XY0 -> 0x50 = Skips the next instruction if VX equals VY.
     *  0x6XNN -> 0x6  = Sets VX to NN.
     *  0x7XNN -> 0x7  = Adds NN to VX. (Carry flag is not changed)
     *  0x8XY0 -> 0x80 = Sets VX to the value of VY.
     *  0x8XY1 -> 0x81 = Sets VX to VX or VY. (Bitwise OR operation)
     *  0x8XY2 -> 0x82 = Sets VX to VX and VY. (Bitwise AND operation)
     *  0x8XY3 -> 0x83 = Sets VX to VX xor VY.
     *  0x8XY4 -> 0x84 = Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
     *  0x8XY5 -> 0x85 = VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
     *  0x8XY6 -> 0x86 = Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
     *  0x8XY7 -> 0x87 = Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
     *  0x8XYE -> 0x8E = Stores the most significant bit of VX in VF and then shifts VX to the left by 1.[b].
     *  0x9XY0 -> 0x90 = Skips the next instruction if VX doesn't equal VY. 
     *  0xANNN -> 0xA  = Sets I to the address NNN.
     *  0xBNNN -> 0xB  = Jumps to the address NNN plus V0.
     *  0xCXNN -> 0xC  = Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
     *  0xDXYN -> 0xD  = Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N+1 pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
     *  0xEX9E -> 0xE9E = Skips the next instruction if the key stored in VX is pressed
     *  0xEXA1 -> 0xEA1 = Skips the next instruction if the key stored in VX isn't pressed
     *  0xFX07 -> 0xF07 = Sets VX to the value of the delay timer.
     *  0xFX0A -> 0xF0A = A key press is awaited, and then stored in VX
     *  0xFX15 -> 0xF15 = Sets the delay timer to VX.
     *  0xFX18 -> 0xF18 = Sets the sound timer to VX.
     *  0xFX1E -> 0xF1E = Adds VX to I. VF is not affected.
     *  0xFX29 -> 0xF29 = Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
     *  0xFX33 -> 0xF33 = Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
     *  0xFX55 -> 0xF55 = Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
     *  0xFX65 -> 0xF65 = Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d]
     *
     *  @tparam N the number of bytes in the buffer program.
     *  
     *  @param program the buffer that contains the binary data.
     *  @param PC the program counter which tells us where is the next OpCode.
     *  
     *  @return a OpCode struct that contains the translated OpCode and its data.
     *  
     *  @throw index out of bounds exception if for some reason the buffer is not multiple of 2.
     */
    template <size_t N>
    static inline OpCode decode(const std::array<uint8_t, N>& program, uint16_t PC)
    {
        const uint8_t first_half  = program[PC];
        const uint8_t second_half = program[PC + 1];

        uint16_t code = (first_half & 0xF0) >> 0x4;
        uint16_t data = 0x0;

        switch (code)
        {
        case 0x5:
        case 0x8:
        case 0x9:
            code = (code << 0x4) | (second_half & 0xF);
            data = ((first_half & 0xF) << 0x4) | ((second_half & 0xF0) >> 0x4);
            break;
        case 0xE:
        case 0xF:
            code = (code << 0x8) | second_half;
            data = first_half & 0xF;
            break;
        case 0x0:
            code = second_half;   
            break;
        default:
            data = ((first_half & 0xF) << 0x8) | second_half; 
            break;
        }

        return OpCode{ code , data };  
    }
}

#endif