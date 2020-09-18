#include <gtest/gtest.h>
#include <fstream>

#include "../include/opcode.h"
#include "../include/disassembler.h"

TEST(DisassemblerTest, CanDisassemble0xE0)
{
    chip::OpCode op_code{};
    op_code.code = 0xE0;
    op_code.data = 0x0;

    std::string result = disassemble_0xE0(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "CLS");
}

TEST(DisassemblerTest, CanDisassemble0xEE)
{
    chip::OpCode op_code{};
    op_code.code = 0xEE;
    op_code.data = 0x0;

    std::string result = disassemble_0xEE(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "RET");
}

TEST(DisassemblerTest, CanDisassemble0x1)
{
    chip::OpCode op_code{};
    op_code.code = 0x1;
    op_code.data = 0x223;

    std::string result = disassemble_0x1(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "JMP\t$223");
}

TEST(DisassemblerTest, CanDisassemble0x2 )
{
    chip::OpCode op_code{};
    op_code.code = 0x2;
    op_code.data = 0x322;

    std::string result = disassemble_0x2(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "CALL\t$322");
}

TEST(DisassemblerTest, CanDisassemble0x3 )
{
    chip::OpCode op_code{};
    op_code.code = 0x3;
    op_code.data = 0x11;

    std::string result = disassemble_0x3(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SE\tV0, $11");
}

TEST(DisassemblerTest, CanDisassemble0x4 )
{
    chip::OpCode op_code{};
    op_code.code = 0x4;
    op_code.data = 0x132;

    std::string result = disassemble_0x4(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SNE\tV1, $32");
}

TEST(DisassemblerTest, CanDisassemble0x50)
{
    chip::OpCode op_code{};
    op_code.code = 0x50;
    op_code.data = 0x21;

    std::string result = disassemble_0x50(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SE\tV2, V1");
}

TEST(DisassemblerTest, CanDisassemble0x6)
{
    chip::OpCode op_code{};
    op_code.code = 0x6;
    op_code.data = 0x311;

    std::string result = disassemble_0x6(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "MOV\tV3, $11");
}

TEST(DisassemblerTest, CanDisassemble0x7)
{
    chip::OpCode op_code{};
    op_code.code = 0x7;
    op_code.data = 0x40F;

    std::string result = disassemble_0x7(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "ADD\tV4, $F");
}

TEST(DisassemblerTest, CanDisassemble0x80)
{
    chip::OpCode op_code{};
    op_code.code = 0x80;
    op_code.data = 0x51;

    std::string result = disassemble_0x80(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "MOV\tV5, V1");
}

TEST(DisassemblerTest, CanDisassemble0x81)
{
    chip::OpCode op_code{};
    op_code.code = 0x81;
    op_code.data = 0x61;

    std::string result = disassemble_0x81(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "OR\tV6, V1");
}

TEST(DisassemblerTest, CanDisassemble0x82)
{
    chip::OpCode op_code{};
    op_code.code = 0x82;
    op_code.data = 0x71;

    std::string result = disassemble_0x82(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "AND\tV7, V1");
}

TEST(DisassemblerTest, CanDisassemble0x83)
{
    chip::OpCode op_code{};
    op_code.code = 0x83;
    op_code.data = 0x81;

    std::string result = disassemble_0x83(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "XOR\tV8, V1");
}

TEST(DisassemblerTest, CanDisassemble0x84)
{
    chip::OpCode op_code{};
    op_code.code = 0x84;
    op_code.data = 0x91;

    std::string result = disassemble_0x84(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "ADD\tV9, V1");
}

TEST(DisassemblerTest, CanDisassemble0x85)
{
    chip::OpCode op_code{};
    op_code.code = 0x85;
    op_code.data = 0x1;

    std::string result = disassemble_0x85(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SUB\tV0, V1");
}

TEST(DisassemblerTest, CanDisassemble0x86)
{
    chip::OpCode op_code{};
    op_code.code = 0x86;
    op_code.data = 0x11;

    std::string result = disassemble_0x86(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SHR\tV1");
}

TEST(DisassemblerTest, CanDisassemble0x87)
{
    chip::OpCode op_code{};
    op_code.code = 0x87;
    op_code.data = 0x21;

    std::string result = disassemble_0x87(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SUBN\tV2, V1");  
}

TEST(DisassemblerTest, CanDisassemble0x8E)
{
    chip::OpCode op_code{};
    op_code.code = 0x8E;
    op_code.data = 0x31;

    std::string result = disassemble_0x8E(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SHL\tV3");
}

TEST(DisassemblerTest, CanDisassemble0x90)
{
    chip::OpCode op_code{};
    op_code.code = 0x90;
    op_code.data = 0x41;

    std::string result = disassemble_0x90(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SNE\tV4, V1");
}

TEST(DisassemblerTest, CanDisassemble0xA )
{
    chip::OpCode op_code{};
    op_code.code = 0xA;
    op_code.data = 0x12;

    std::string result = disassemble_0xA(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "MVI\tI $12");
}

TEST(DisassemblerTest, CanDisassemble0xB )
{
    chip::OpCode op_code{};
    op_code.code = 0xB;
    op_code.data = 0x123;

    std::string result = disassemble_0xB(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "JMP\tV0, $123");
}

TEST(DisassemblerTest, CanDisassemble0xC )
{
    chip::OpCode op_code{};
    op_code.code = 0xC;
    op_code.data = 0x233;

    std::string result = disassemble_0xC(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "RND\tV2, $33");
}

TEST(DisassemblerTest, CanDisassemble0xD )
{
    chip::OpCode op_code{};
    op_code.code = 0xD;
    op_code.data = 0x114;

    std::string result = disassemble_0xD(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "DRW\tV1, V1, $4");
}

TEST(DisassemblerTest, CanDisassemble0xE9E)
{
    chip::OpCode op_code{};
    op_code.code = 0xE9E;
    op_code.data = 0x1;

    std::string result = disassemble_0xE9E(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SKP\tV1");
}

TEST(DisassemblerTest, CanDisassemble0xEA1)
{
    chip::OpCode op_code{};
    op_code.code = 0xEA1;
    op_code.data = 0x1;

    std::string result = disassemble_0xEA1(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "SKNP\tV1");
}

TEST(DisassemblerTest, CanDisassemble0xF07)
{
    chip::OpCode op_code{};
    op_code.code = 0xF07;
    op_code.data = 0x1;

    std::string result = disassemble_0xF07(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tV1, DT");
}

TEST(DisassemblerTest, CanDisassemble0xF0A)
{
    chip::OpCode op_code{};
    op_code.code = 0xF0A;
    op_code.data = 0x1;

    std::string result = disassemble_0xF0A(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tV1, K");
}

TEST(DisassemblerTest, CanDisassemble0xF15)
{
    chip::OpCode op_code{};
    op_code.code = 0xF15;
    op_code.data = 0x1;

    std::string result = disassemble_0xF15(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tDT, V1");
} 

TEST(DisassemblerTest, CanDisassemble0xF18)
{
    chip::OpCode op_code{};
    op_code.code = 0xF18;
    op_code.data = 0x1;

    std::string result = disassemble_0xF18(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tST, V1");
}

TEST(DisassemblerTest, CanDisassemble0xF1E)
{
    chip::OpCode op_code{};
    op_code.code = 0xF1E;
    op_code.data = 0x1;

    std::string result = disassemble_0xF1E(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "ADD\tI, V1");
}

TEST(DisassemblerTest, CanDisassemble0xF29)
{
    chip::OpCode op_code{};
    op_code.code = 0xF29;
    op_code.data = 0x1;

    std::string result = disassemble_0xF29(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tF, V1");
}

TEST(DisassemblerTest, CanDisassemble0xF33)
{
    chip::OpCode op_code{};
    op_code.code = 0xF33;
    op_code.data = 0x1;

    std::string result = disassemble_0xF33(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tB, V1");
}

TEST(DisassemblerTest, CanDisassemble0xF55)
{
    chip::OpCode op_code{};
    op_code.code = 0xF55;
    op_code.data = 0x1;

    std::string result = disassemble_0xF55(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\t[I], V1");
}

TEST(DisassemblerTest, CanDisassemble0xF65)
{
    chip::OpCode op_code{};
    op_code.code = 0xF65;
    op_code.data = 0x1;
    
    std::string result = disassemble_0xF65(op_code);

    EXPECT_STRCASEEQ(result.c_str(), "LD\tV1, [I]");
}

TEST(DisassemblerTest, CanDisassembleBinary)
{
    std::array<uint8_t, 246> program 
    {{
        0x6a, 0x02, 0x6b, 0x0c, 0x6c, 0x3f, 0x6d, 0x0c, 0xa2, 0xea, 0xda, 0xb6, 0xdc, 0xd6, 0x6e, 0x00,
        0x22, 0xd4, 0x66, 0x03, 0x68, 0x02, 0x60, 0x60, 0xf0, 0x15, 0xf0, 0x07, 0x30, 0x00, 0x12, 0x1a,
        0xc7, 0x17, 0x77, 0x08, 0x69, 0xff, 0xa2, 0xf0, 0xd6, 0x71, 0xa2, 0xea, 0xda, 0xb6, 0xdc, 0xd6,
        0x60, 0x01, 0xe0, 0xa1, 0x7b, 0xfe, 0x60, 0x04, 0xe0, 0xa1, 0x7b, 0x02, 0x60, 0x1f, 0x8b, 0x02,
        0xda, 0xb6, 0x60, 0x0c, 0xe0, 0xa1, 0x7d, 0xfe, 0x60, 0x0d, 0xe0, 0xa1, 0x7d, 0x02, 0x60, 0x1f,
        0x8d, 0x02, 0xdc, 0xd6, 0xa2, 0xf0, 0xd6, 0x71, 0x86, 0x84, 0x87, 0x94, 0x60, 0x3f, 0x86, 0x02,
        0x61, 0x1f, 0x87, 0x12, 0x46, 0x02, 0x12, 0x78, 0x46, 0x3f, 0x12, 0x82, 0x47, 0x1f, 0x69, 0xff,
        0x47, 0x00, 0x69, 0x01, 0xd6, 0x71, 0x12, 0x2a, 0x68, 0x02, 0x63, 0x01, 0x80, 0x70, 0x80, 0xb5,
        0x12, 0x8a, 0x68, 0xfe, 0x63, 0x0a, 0x80, 0x70, 0x80, 0xd5, 0x3f, 0x01, 0x12, 0xa2, 0x61, 0x02,
        0x80, 0x15, 0x3f, 0x01, 0x12, 0xba, 0x80, 0x15, 0x3f, 0x01, 0x12, 0xc8, 0x80, 0x15, 0x3f, 0x01,
        0x12, 0xc2, 0x60, 0x20, 0xf0, 0x18, 0x22, 0xd4, 0x8e, 0x34, 0x22, 0xd4, 0x66, 0x3e, 0x33, 0x01,
        0x66, 0x03, 0x68, 0xfe, 0x33, 0x01, 0x68, 0x02, 0x12, 0x16, 0x79, 0xff, 0x49, 0xfe, 0x69, 0xff,
        0x12, 0xc8, 0x79, 0x01, 0x49, 0x02, 0x69, 0x01, 0x60, 0x04, 0xf0, 0x18, 0x76, 0x01, 0x46, 0x40,
        0x76, 0xfe, 0x12, 0x6c, 0xa2, 0xf2, 0xfe, 0x33, 0xf2, 0x65, 0xf1, 0x29, 0x64, 0x14, 0x65, 0x00,
        0xd4, 0x55, 0x74, 0x15, 0xf2, 0x29, 0xd4, 0x55, 0x00, 0xee, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00
    }};

        std::ofstream file;
        file.open("pong.txt");

        if(file.is_open())
        {
            chip::disassemble(program, file);
        
            file.close();

            ASSERT_TRUE(true);
        }
        else
        {
            ASSERT_TRUE(false);
        }
        
}