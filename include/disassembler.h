#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <array>
#include <string>
#include <memory>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "./opcode.h"
#include "./cpu.h"

namespace chip
{

    /**
     *  On this file we present a collection of functions that 
     *  map an OpCode into an assembly instruction. For one thing,
     *  every OpCode has its own function so we can properly get
     *  the corresponding assembly code and decode the data as
     *  required.
     */ 

    static inline std::string disassemble_0xE0(const OpCode& op_code)
    {
        return {"CLS"};
    }

    static inline std::string disassemble_0xEE(const OpCode& op_code)
    {
        return {"RET"};
    }

    static inline std::string disassemble_0x0(const OpCode& op_code)
    {
        return {"NOP"};
    }
    
    static inline std::string disassemble_0x1(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "JMP\t$" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0x2(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "CALL\t$" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0x3(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "SE\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x4(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "SNE\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x50(const OpCode& op_code)
    {
        std::stringstream result; 
        std::stringstream y_register;

        result << std::hex << "SE\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x6(const OpCode& op_code)
    {
        std::stringstream result; 
        std::stringstream value;

        result << std::hex << "MOV\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x7(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "ADD\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x80(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "MOV\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x81(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "OR\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x82(const OpCode& op_code)
    {   
        std::stringstream result; 
        
        result << std::hex << "AND\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x83(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "XOR\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x84(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "ADD\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x85(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "SUB\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x86(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "SHR\tV" << (int)((op_code.data & 0xF0) >> 4);

        return result.str();
    }

    static inline std::string disassemble_0x87(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "SUBN\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x8E(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "SHL\tV" << (int)((op_code.data & 0xF0) >> 4);

        return result.str();
    }

    static inline std::string disassemble_0x90(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "SNE\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);
    
        return result.str();
    }

    static inline std::string disassemble_0xA(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "MVI\tI $" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0xB(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "JMP\tV0, $" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0xC(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "RND\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);;

        return result.str();
    }

    static inline std::string disassemble_0xD(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "DRW\tV" << (int)((op_code.data & 0xF00) >> 8) << ", V" << (int)((op_code.data & 0xF0)  >> 4) <<  ", $" << (op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0xE9E(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "SKP\tV" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xEA1(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "SKNP\tV" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF07(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tV" << (int)(op_code.data) << ", DT";

        return result.str();
    }

    static inline std::string disassemble_0xF0A(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tV" << (int)(op_code.data) << ", K";

        return result.str();
    }

    static inline std::string disassemble_0xF15(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tDT, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF18(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tST, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF1E(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "ADD\tI, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF29(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tF, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF33(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tB, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF55(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\t[I], V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF65(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "LD\tV" << (int)(op_code.data) << ", [I]";

        return result.str();
    }

    /**
     *  Convert every OpCode in the binary buffer to its corresponding
     *  assembly counterpart and decode the OpCode data so we can 
     *  properly see how is the program working.
     *  
     *  @param program a buffer that contains the loaded program.
     * 
     */ 
    template <size_t N>
    void disassemble(const std::array<uint8_t, N>& program, std::ofstream& output)
    {
        uint32_t PC = 0;

        const std::unordered_map<uint16_t, std::function<std::string (const OpCode&)>> disassemblers
        {
            { 0xE0,  disassemble_0xE0 },
            { 0xEE,  disassemble_0xEE },
            { 0x0 ,  disassemble_0x0  }, 
            { 0x1,   disassemble_0x1  }, 
            { 0x2,   disassemble_0x2  }, 
            { 0x3,   disassemble_0x3  }, 
            { 0x4,   disassemble_0x4  }, 
            { 0x50,  disassemble_0x50 }, 
            { 0x6,   disassemble_0x6  }, 
            { 0x7,   disassemble_0x7  }, 
            { 0x80,  disassemble_0x80 }, 
            { 0x81,  disassemble_0x81 }, 
            { 0x82,  disassemble_0x82 }, 
            { 0x83,  disassemble_0x83 }, 
            { 0x84,  disassemble_0x84 }, 
            { 0x85,  disassemble_0x85 }, 
            { 0x86,  disassemble_0x86 }, 
            { 0x87,  disassemble_0x87 }, 
            { 0x8E,  disassemble_0x8E }, 
            { 0x90,  disassemble_0x90 }, 
            { 0xA,   disassemble_0xA }, 
            { 0xB,   disassemble_0xB }, 
            { 0xC,   disassemble_0xC }, 
            { 0xD,   disassemble_0xD }, 
            { 0xE9E, disassemble_0xE9E }, 
            { 0xEA1, disassemble_0xEA1 }, 
            { 0xF07, disassemble_0xF07 }, 
            { 0xF0A, disassemble_0xF0A }, 
            { 0xF15, disassemble_0xF15 }, 
            { 0xF18, disassemble_0xF18 }, 
            { 0xF1E, disassemble_0xF1E }, 
            { 0xF29, disassemble_0xF29 }, 
            { 0xF33, disassemble_0xF33 }, 
            { 0xF55, disassemble_0xF55 }, 
            { 0xF65, disassemble_0xF65 }
        };

        int line = 0;

        output << "ADDR" << "  " << "Assembly" << "\n";
        output << "----" << "  " << "--------" << "\n";

        while(PC < program.size())
        {
            OpCode op_code = decode(program, PC);
            
            std::string message = disassemblers.at(op_code.code)(op_code);
            
            output <<  std::setfill('0') << std::setw(4) << std::to_string(line) << "  " << message << "\n";
            
            PC += 2; line++;
        }
    }
}

#endif