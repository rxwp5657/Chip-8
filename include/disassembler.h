#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <array>
#include <string>
#include <memory>
#include <cstdint>
#include <fstream>
#include <sstream>
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
        return {"(E0)\tCLS"};
    }

    static inline std::string disassemble_0xEE(const OpCode& op_code)
    {
        return {"(EE)\tRET"};
    }

    static inline std::string disassemble_0x0(const OpCode& op_code)
    {
        return {"(0)\tNOP"};
    }
    
    static inline std::string disassemble_0x1(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(1)\tJMP\t$" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0x2(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(2)\tCALL\t$" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0x3(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(3)\tSE\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x4(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(4)\tSNE\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x50(const OpCode& op_code)
    {
        std::stringstream result; 
        std::stringstream y_register;

        result << std::hex << "(50)\tSE\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x6(const OpCode& op_code)
    {
        std::stringstream result; 
        std::stringstream value;

        result << std::hex << "(6)\tMOV\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x7(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(7)\tADD\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);

        return result.str();
    }

    static inline std::string disassemble_0x80(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(80)\tMOV\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x81(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(81)\tOR\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x82(const OpCode& op_code)
    {   
        std::stringstream result; 
        
        result << std::hex << "(82)\tAND\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x83(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(83)\tXOR\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x84(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(84)\tADD\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x85(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(85)\tSUB\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x86(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(86)\tSHR\tV" << (int)((op_code.data & 0xF0) >> 4);

        return result.str();
    }

    static inline std::string disassemble_0x87(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(87)\tSUBN\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0x8E(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(8E)\tSHL\tV" << (int)((op_code.data & 0xF0) >> 4);

        return result.str();
    }

    static inline std::string disassemble_0x90(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(90)\tSNE\tV" << (int)((op_code.data & 0xF0) >> 4) << ", V" << (int)(op_code.data & 0xF);
    
        return result.str();
    }

    static inline std::string disassemble_0xA(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(A)\tMVI\tI $" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0xB(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(B)\tJMP\tV0, $" << (int)op_code.data;

        return result.str();
    }

    static inline std::string disassemble_0xC(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(C)\tRND\tV" << (int)((op_code.data & 0xF00) >> 8) << ", $" << (int)(op_code.data & 0xFF);;

        return result.str();
    }

    static inline std::string disassemble_0xD(const OpCode& op_code)
    {
        std::stringstream result; 

        result << std::hex << "(D)\tDRW\tV" << (int)((op_code.data & 0xF00) >> 8) << ", V" << (int)((op_code.data & 0xF0)  >> 4) <<  ", $" << (op_code.data & 0xF);

        return result.str();
    }

    static inline std::string disassemble_0xE9E(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(E9E)\tSKP\tV" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xEA1(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(EA1)\tSKNP\tV" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF07(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F07)\tLD\tV" << (int)(op_code.data) << ", DT";

        return result.str();
    }

    static inline std::string disassemble_0xF0A(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F0A)\tLD\tV" << (int)(op_code.data) << ", K";

        return result.str();
    }

    static inline std::string disassemble_0xF15(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F15)\tLD\tDT, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF18(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F18)\tLD\tST, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF1E(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F1E)\tADD\tI, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF29(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F29)\tLD\tF, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF33(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F33)\tLD\tB, V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF55(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F55)\tLD\t[I], V" << (int)(op_code.data);

        return result.str();
    }

    static inline std::string disassemble_0xF65(const OpCode& op_code)
    {
        std::stringstream result;

        result << std::hex << "(F65)\tLD\tV" << (int)(op_code.data) << ", [I]";

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
            
            if(disassemblers.find(op_code.code) != disassemblers.end())
            {
                std::string message = disassemblers.at(op_code.code)(op_code);
                output <<  std::setfill('0') << std::setw(4) << std::to_string(line) << "  " << message << "\n";
                line++;
            }
            PC += 2;
        }
    }
}

#endif