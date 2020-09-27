#ifndef CPU_H
#define CPU_H

#include <tuple>
#include <array>
#include <time.h> 
#include <string>
#include <fstream>
#include <cstdint>
#include <stdlib.h>
#include <functional>
#include <unordered_map>

#include "./opcode.h"

namespace chip
{
    /**
     *  Chip-8 programs start at address 0x200;
     */ 
    const uint16_t ROM_START = 0x200;

    /**
     *  Chip-8 has a font set which can be used to 
     *  write letters A - F and digits 0 - 9. Also,
     *  each character is 4 pixels wide and 5 pixels
     *  high.
     */
    const std::array<uint8_t, 80> font_set 
    {{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0x0
        0x20, 0x60, 0x20, 0x20, 0x70, // 0x1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 0x2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 0x3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 0x4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 0x5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 0x6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 0x7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 0x8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 0x9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // 0xA
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // 0xB
        0xF0, 0x80, 0x80, 0x80, 0xF0, // 0xC
        0xE0, 0x90, 0x90, 0x90, 0xE0, // 0xD
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // 0xE
        0xF0, 0x80, 0xF0, 0x80, 0x80  // 0xF
    }};

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
        CPU() : DT{0}, ST{0}, SP{0}, PC{0x200},I{0}, V{}, key_pad{}, memory{}, screen{}, stack{} {}
        uint8_t  DT; // Delay Time register.
        uint8_t  ST; // Sound Time register.    
        uint8_t  SP; // Stack Pointer.
        uint16_t PC; // Program Counter.
        uint16_t I;  // Special register I.
        std::array<uint8_t, 16> V; // General purpose registers (Vx).
        std::array<uint8_t, 16> key_pad;
        std::array<uint8_t, 4096> memory; // memory of the program.
        std::array<uint8_t, 64 * 32> screen; // Chip-8 expects a screen of 64 by 32 pixels.
        std::array<uint16_t,16> stack; // stack for function call.
    };

    /**
     * Load the character font set into memory.
     * 
     * @param cpu load the font set into the memory.
     */ 
    static inline void load_font_set(CPU& cpu)
    {
        for(int i = 0 ; i < font_set.size() ; i++) cpu.memory[i] = font_set[i];
    }

    /**
     *  Do nothing.
     */  
    static inline void op_code_0x0(CPU& cpu, const OpCode& op_code)
    {

    }

    /**
     *  Clear the pixels of the screen with the color black.
     *  
     *  @param cpu the cpu that holds the screen to be cleared.
     */
    static inline void op_code_0xE0(CPU& cpu, const OpCode& op_code)
    {
        for(int i = 0 ; i < cpu.screen.size() ; i++) cpu.screen[i] = 0;
    }

    /**
     *  Restore the PC register with the address that it used to
     *  point after issuing a CALL OpCode (call to a subroutine).
     *  After restoring PC, decrement the stack pointer (SP) by 
     *  one so the space can be used.
     * 
     *  @param cpu the cpu whose PC register will be restored.
     */ 
    static inline void op_code_0xEE(CPU& cpu, const OpCode& op_code)
    {
        cpu.PC = cpu.stack[cpu.SP - 1];
        cpu.SP -= 1;
    }

    /**
     *  Set the PC register value with the address specified in the
     *  OpCode (Jumps to address NNN).
     * 
     *  @param cpu the cpu whose PC register will be set.
     *  @param op_code information needed by the op_code (e.g. the address 
     *                 which the PC register will jump to).
     */ 
    static inline void op_code_0x1(CPU& cpu, const OpCode& op_code)
    {
        cpu.PC = op_code.data;
    }


    /**
     *  Push the current address in PC into the stack and set the 
     *  PC register value with the address specified in the OpCode.
     *  Also, it will increment the stack pointer (SP) by one so 
     *  it points to a free space (Calls subroutine at NNN).
     * 
     *  @param cpu the cpu whose PC register will be set.
     *  @param op_code contains the address where we want to jump to.
     */ 
    static inline void op_code_0x2(CPU& cpu, const OpCode& op_code)
    {
        cpu.stack[cpu.SP] = cpu.PC;
        cpu.PC = op_code.data;
        cpu.SP += 1;
    }

    /**
     *  Skips the next instruction if the value on the register
     *  Vx is equal to the value kk (both are encoded into the
     *  data of the op_code).
     * 
     *  @param cpu the cpu whose PC register will be incremented.
     *  @param op_code contains the id of the register and the value
     *                 that will be compared.
     */ 
    static inline void op_code_0x3(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[((op_code.data & 0xF00) >> 8)] == (op_code.data & 0xFF)) cpu.PC += 2;
    }
    
    /**
     *  Skips the next instruction if the value on the register
     *  Vx is not equal to NN (both are encoded into the
     *  data of the op_code).
     * 
     *  @param cpu the cpu whose PC register will be incremented.
     *  @param op_code contains the id of the register and the value
     *                 that will be compared.
     */ 
    static inline void op_code_0x4(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[((op_code.data & 0xF00) >> 8)] != (op_code.data & 0xFF)) cpu.PC += 2;
    }

    /**
     *  Skips the next instruction if the value of the register 
     *  Vx is equal to the value of the register VY.
     * 
     *  @param cpu the cpu whose PC register will be incremented.
     *  @param op_code contains the id of the register and the value
     *                 that will be compared.
     */ 
    static inline void op_code_0x50(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[((op_code.data & 0xF0) >> 4)] == cpu.V[(op_code.data & 0xF)]) cpu.PC += 2;
    }

    /**
     *  Sets the value of the register Vx to the one specified
     *  by the data on the OpCode
     * 
     *  @param cpu the cpu whose Vx register will be set.
     *  @param op_code contains the id of the register and the value
     *                 that will be set.
     */ 
    static inline void op_code_0x6(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[((op_code.data & 0xF00) >> 8)] = (op_code.data & 0xFF); 
    }

    /**
     *  Adds the value in register Vx with the value kk specified on
     *  the OpCode.
     * 
     *  @param cpu the cpu whose Vx register will be added.
     *  @param op_code contains the kk value to be added.
     */ 
    static inline void op_code_0x7(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[((op_code.data & 0xF00) >> 8)] = cpu.V[((op_code.data & 0xF00) >> 8)] + (op_code.data & 0xFF);
    }

    /**
     *  Stores the value on register Vy into register Vx.
     * 
     *  @param cpu the cpu containing the registers.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0x80(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[(op_code.data & 0xF)];
    }

    /**
     *  Performs a bitwise OR operation between the values 
     *  of the registers Vx and Vy and stores the result into
     *  Vx.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0x81(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] | cpu.V[op_code.data & 0xF];
    }

     /**
     *  Performs a bitwise AND operation between the values 
     *  of the registers Vx and Vy and stores the result into
     *  Vx.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0x82(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] & cpu.V[op_code.data & 0xF];
    }

     /**
     *  Performs a bitwise XOR operation between the values 
     *  of the registers Vx and Vy and stores the result into
     *  Vx.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0x83(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] ^ cpu.V[op_code.data & 0xF];
    }

     /**
     *  Adds the values on registers Vx and Vy and stores the result
     *  on the Vx register. However, if the addition is greater than 
     *  255 the VF register will be set to one (carry) and the register
     *  Vx keeps the 8 lower bits.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0x84(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[((op_code.data & 0xF0) >> 4)] + cpu.V[op_code.data & 0xF] > 255)
        {
            cpu.V[0xF] = 0x1;
        }
        else 
        {
            cpu.V[0xF] = 0x0;
        }
        
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] + cpu.V[(op_code.data & 0xF)];
    }   

     /**
     *  Substracts the value on register Vx by the value on register Vy 
     *  and stores the result on the Vx register. However, if the value 
     *  on the Vx register is greater than the value on the Vy register then 
     *  the VF register will be set to one (carry) and the register Vx 
     *  keeps the 8 lower bits.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */
    static inline void op_code_0x85(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[((op_code.data & 0xF0) >> 4)] > cpu.V[op_code.data & 0xF])
        {
            cpu.V[0xF] = 0x1;
        }
        else
        {
            cpu.V[0xF] = 0x0;
        }
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] - cpu.V[(op_code.data & 0xF)];
    }

    /**
     *  Shifts the value of the Vx register one to the right but, before
     *  shifting it stores the least significant bit of the value on the
     *  register VF. 
     * 
     *  @param cpu the cpu that contains the register used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0x86(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[0xF] = cpu.V[((op_code.data & 0xF0) >> 4)] & 0x1;
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] >> 1;
    }

    /**
     *  Substracts the value on register Vy by the value on register Vx 
     *  and stores the result on the Vx register. However, if the value 
     *  on the Vy register is greater than the value on the Vx register then 
     *  the VF register will be set to one (carry) and the register Vx 
     *  keeps the 8 lower bits.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */
    static inline void op_code_0x87(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[op_code.data & 0xF] > cpu.V[((op_code.data & 0xF0) >> 4)])
        {
            cpu.V[0xF] = 0x1;
        }
        else
        {
            cpu.V[0xF] = 0x0;
        }
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[(op_code.data & 0xF)] - cpu.V[((op_code.data & 0xF0) >> 4)];
    }

    /**
     *  Shifts the value of the Vx register one to the left but, before
     *  shifting it stores the most significant bit of the value on the
     *  register VF. 
     * 
     *  @param cpu the cpu that contains the register used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0x8E(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[0xF] = cpu.V[((op_code.data & 0xF0) >> 4)] & (0x1 << 0x8);
        cpu.V[((op_code.data & 0xF0) >> 4)] = cpu.V[((op_code.data & 0xF0) >> 4)] << 1;
    }

    /**
     *  Skips the next instruction if the value on register Vx doens't equal
     *  to the value on register Vy.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0x90(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.V[((op_code.data & 0xF0) >> 4)] != cpu.V[(op_code.data & 0xF)]) cpu.PC += 2;
    }

    /**
     *  Set the register I to the value NNN of the OpCode.
     * 
     *  @param cpu the cpu that contains the register used by the operation.
     *  @param op_code contains the value that the register will be set.
     */ 
    static inline void op_code_0xA(CPU& cpu, const OpCode& op_code)
    {
        cpu.I = op_code.data;
    }

    /**
     *  Jump to the NNN address plus the value on the register V0.
     * 
     *  @param cpu the cpu that contains the register used by the operation.
     *  @param op_code contains the address to jump to.
     */ 
    static inline void op_code_0xB(CPU& cpu, const OpCode& op_code)
    {
        cpu.PC = cpu.V[0] + op_code.data;
    }

    /**
     *  Set the register Vx to the result of the bitwise operation between
     *  a random number and NN.
     * 
     *  @param cpu the cpu that contains the register used to store the result.
     *  @param op_code contains the id of the register and the value to be used
     *                 by the operation.
     */ 
    static inline void op_code_0xC(CPU& cpu, const OpCode& op_code)
    {
        uint8_t random = rand() % 256;

        cpu.V[(op_code.data & 0xF00) >> 8] = (op_code.data & 0xFF) & random; 
    }

    /**
     *  Draws a sprite at coordinate (VX, VY) that has a width of 8 
     *  pixels and a height of N+1 pixels. Each row of 8 pixels is 
     *  read as bit-coded starting from memory location I; I value 
     *  doesn’t change after the execution of this instruction
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0xD(CPU& cpu, const OpCode& op_code)
    {
        const uint8_t vx = cpu.V[(op_code.data & 0xF00) >> 8];
        const uint8_t vy = cpu.V[(op_code.data & 0xF0)  >> 4];
        const uint8_t h  = op_code.data & 0xF;

        cpu.V[0xF] = 0x0;
        
        for(int i = 0 ; i < h ; i++)
        {
            uint8_t row = cpu.V[cpu.I + i];

            for(int j = 0 ; j < 8 ; j++)
            {
                if((row & (0x80 >> j)) != 0x0)
                {
                    if(cpu.screen[j + vx + ((i + vy) * 64)] == 0x1) cpu.V[0xF] = 0x1;
                    cpu.screen[j + vx + ((i + vy) * 64)] ^= 0x1;
                }            
            }
        }
    }

     /**
     *  Skips the next instruction if the key stored in VX is pressed.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0xE9E(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.key_pad[cpu.V[op_code.data]] == 0x1) cpu.PC += 2;
    }

    /**
     *  Skips the next instruction if the key stored in VX isn't pressed.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0xEA1(CPU& cpu, const OpCode& op_code)
    {
        if(cpu.key_pad[cpu.V[op_code.data]] != 0x1) cpu.PC += 2;
    }

    /**
     *  Skips the next instruction if the key stored in VX isn't pressed.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0xF07(CPU& cpu, const OpCode& op_code)
    {
        cpu.V[op_code.data] = cpu.DT;
    }

    /**
     *  A key press is awaited, and then stored in VX. (Blocking Operation. 
     *  All instruction halted until next key event)
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the registers that will be used.
     */ 
    static inline void op_code_0xF0A(CPU& cpu, const OpCode& op_code)
    {
        for(int i = 0 ; i < 16 ; i++) 
        {
            if(cpu.key_pad[i] != 0)
            {
                cpu.V[op_code.data] = i;
                return;
            }
        }

        cpu.PC -= 2;
    }

    /**
     *  Sets the delay timer to VX.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF15(CPU& cpu, const OpCode& op_code)
    {
        cpu.DT = cpu.V[op_code.data];
    }

    /**
     *  Sets the sound timer to VX.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF18(CPU& cpu, const OpCode& op_code)
    {
        cpu.ST = cpu.V[op_code.data];
    }

    /**
     *  Adds VX to I. VF is not affected
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF1E(CPU& cpu, const OpCode& op_code)
    {
        cpu.I += cpu.V[op_code.data];
    }

    /**
     *  Sets I to the location of the sprite for the character in VX. 
     *  Characters 0-F (in hexadecimal) are represented by a 4x5 font.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF29(CPU& cpu, const OpCode& op_code)
    {
        cpu.I = op_code.data * 5;
    }

    /**
     *  Stores the binary-coded decimal representation of VX, 
     *  with the most significant of three digits at the address 
     *  in I, the middle digit at I plus 1, and the least significant 
     *  digit at I plus 2. (In other words, take the decimal representation 
     *  of VX, place the hundreds digit in memory at location in I, the 
     *  tens digit at location I+1, and the ones digit at location I+2.)
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF33(CPU& cpu, const OpCode& op_code)
    {
        uint8_t data = cpu.V[op_code.data];
        
        cpu.memory[cpu.I + 2] = data % 10; 

        data /= 10;
        cpu.memory[cpu.I + 1] = data % 10;

        data /= 10;
        cpu.memory[cpu.I] = data % 10;
    }

    /**
     *  Stores V0 to VX (including VX) in memory starting at address.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF55(CPU& cpu, const OpCode& op_code)
    {
        for(int i = 0; i <= op_code.data; i++) cpu.memory[cpu.I + i] = cpu.V[i];
    }

    /**
     *  Fills V0 to VX (including VX) with values from memory 
     *  starting at address I. The offset from I is increased 
     *  by 1 for each value written, but I itself is left 
     *  unmodified.
     * 
     *  @param cpu the cpu that contains the registers used by the operation.
     *  @param op_code contains the id of the register that will be used.
     */ 
    static inline void op_code_0xF65(CPU& cpu, const OpCode& op_code)
    {
        for(int i = 0; i <= op_code.data; i++) cpu.V[i] = cpu.memory[cpu.I + i];
    }

    static inline void load_ROM(CPU& cpu, const std::string& rom_path)
    {
        std::ifstream file{rom_path.c_str(), std::ios::in | std::ios::binary | std::ios::ate};

        if(!file.is_open()) throw std::runtime_error{"Unable to open ROM " + rom_path};
        std::streampos rom_size = file.tellg();

        char *file_data = new char[rom_size];

        file.seekg(0, std::ios::beg);
        file.read(&file_data[0], rom_size);
        file.close();

        for(int i = 0 ; i < rom_size ; i++) cpu.memory[ROM_START + i] = static_cast<uint8_t>(file_data[i]);

        delete[] file_data;
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

    const std::unordered_map<uint16_t, std::function<void (CPU& cpu, const OpCode& op_code)>> op_codes 
    {
        { 0xE0,  op_code_0xE0 },
        { 0xEE,  op_code_0xEE },
        { 0x0 ,  op_code_0x0  }, 
        { 0x1,   op_code_0x1  }, 
        { 0x2,   op_code_0x2  }, 
        { 0x3,   op_code_0x3  }, 
        { 0x4,   op_code_0x4  }, 
        { 0x50,  op_code_0x50 }, 
        { 0x6,   op_code_0x6  }, 
        { 0x7,   op_code_0x7  }, 
        { 0x80,  op_code_0x80 }, 
        { 0x81,  op_code_0x81 }, 
        { 0x82,  op_code_0x82 }, 
        { 0x83,  op_code_0x83 }, 
        { 0x84,  op_code_0x84 }, 
        { 0x85,  op_code_0x85 }, 
        { 0x86,  op_code_0x86 }, 
        { 0x87,  op_code_0x87 }, 
        { 0x8E,  op_code_0x8E }, 
        { 0x90,  op_code_0x90 }, 
        { 0xA,   op_code_0xA }, 
        { 0xB,   op_code_0xB }, 
        { 0xC,   op_code_0xC }, 
        { 0xD,   op_code_0xD }, 
        { 0xE9E, op_code_0xE9E }, 
        { 0xEA1, op_code_0xEA1 }, 
        { 0xF07, op_code_0xF07 }, 
        { 0xF0A, op_code_0xF0A }, 
        { 0xF15, op_code_0xF15 }, 
        { 0xF18, op_code_0xF18 }, 
        { 0xF1E, op_code_0xF1E }, 
        { 0xF29, op_code_0xF29 }, 
        { 0xF33, op_code_0xF33 }, 
        { 0xF55, op_code_0xF55 }, 
        { 0xF65, op_code_0xF65 }
    };

    /**
     *  Fetch, decode and execute an instruction
     *  from memory. Also, increment the PC register
     *  by two (advance to the next instruction).
     * 
     *  @param cpu it contains all the resources
     *             used by the program.
     */
    static inline void cycle(CPU& cpu)
    {
        OpCode op_code = decode(cpu.memory, cpu.PC);

        op_codes.at(op_code.code)(cpu, op_code);

        cpu.PC += 2;
    }
}

#endif