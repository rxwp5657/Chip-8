#include <gtest/gtest.h>
#include <array>
#include <vector>

#include "../include/opcode.h"
#include "../include/cpu.h"

TEST(CPUTest, CanDecodeChip8Instructions)
{
    uint16_t PC = 0;

    std::array<uint8_t, 68> program 
    {{
        0x00, 0xE0, // 0x00E0 -> clear screen.
        0x00, 0xEE, // 0x00EE -> return from subroutine.
        0x12, 0x23, // 0x1 - 0x223 -> Jump to address 0x223.
        0x23, 0x22, // 0x2 - 0x322 -> Call subroutine at 0x322.
        0x30, 0x11, // 0x3 - V0 == 11 -> Check if the value at register 0 is equal to 11.
        0x41, 0x32, // 0x4 - V1 != 32 -> Check if the value at register 1 is not equal to 32.
        0x52, 0x10, // 0x5 - V2 == V1 -> Check if the value at register 2 is equal to the value at register V1.
        0x63, 0x11, // 0x6 set the value of register V3 to 0x11.
        0x74, 0x0F, // 0x7 add the value 0x0F to the value at register V4 and store the value in V4.
        0x85, 0x10, // 0x8...0 set the value of register V1 into the register V5.
        0x86, 0x11, // 0x8...1 V6 = V6 | V1.
        0x87, 0x12, // 0x8...2 V7 = V7 & V1.
        0x88, 0x13, // 0x8...3 V8 = V8 ^ V1.
        0x89, 0x14, // 0x8...4 V9 = V9 + V1 and if the result is greater than 255 set VF (carry) to 1, otherwise 0.
        0x80, 0x15, // 0x8...5 V0 = V0 - V1 and if Vx > Vy, then VF is set to 1, otherwise 0.
        0x81, 0x16, // 0x8...6 set If the least-significant bit of V1 is 1, then VF is set to 1, otherwise 0. Then V1 is divided by 2.
        0x82, 0x17, // 0x8...7 V2 = V1 - V2 and if 1y > V2, then VF is set to 1, otherwise 0.
        0x83, 0x1E, // 0x8...E If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
        0x94, 0x10, // 0x9...0 Skip next instruction if V4 != V1.
        0xA0, 0x12, // 0xA -> set the value of register I to 0x012.
        0xB1, 0x23, // 0xB -> jump to location 0x123 + V0.
        0xC2, 0x33, // 0xC -> Set Vx = random byte AND kk.
        0xD1, 0x14, // 0xD -> Display n-byte sprite starting at memory location I at (V1, V1), set VF = collision.
        0xE1, 0x9E, // 0xE...9E -> Checks the keyboard, and if the key corresponding to the value of V1 is currently in the down position, PC is increased by 2.
        0xE1, 0xA1, // 0xE...A1 -> Skip next instruction if key with the value of Vx is not pressed.
        0xF1, 0x07, // Set V1 = delay timer value.
        0xF1, 0x0A, // Wait for a key press, store the value of the key in V1.
        0xF1, 0x15, // Set delay timer = V1.
        0xF1, 0x18, // Set sound timer = V1.
        0xF1, 0x1E, // Set I = I + V1.
        0xF1, 0x29, // Set I = location of sprite for digit V1.
        0xF1, 0x33, // Store BCD representation of V1 in memory locations I, I+1, and I+2.
        0xF1, 0x55, // Store registers V0 through V1 in memory starting at location I
        0xF1, 0x65, // Read registers V0 through V1 from memory starting at location I.
    }};

    std::vector<chip::OpCode> expected 
    {
        { 0x00E0, 0x0   },
        { 0x00EE, 0x0   },
        { 0x0001, 0x223 },
        { 0x0002, 0x322 },
        { 0x0003, 0x011 },
        { 0x0004, 0x132 },
        { 0x0050, 0x21  },
        { 0x0006, 0x311 },
        { 0x0007, 0x40F },
        { 0x0080, 0x51  },
        { 0x0081, 0x61  },
        { 0x0082, 0x71  },
        { 0x0083, 0x81  },
        { 0x0084, 0x91  },
        { 0x0085, 0x01  },
        { 0x0086, 0x11  },
        { 0x0087, 0x21  },
        { 0x008E, 0x31  },
        { 0x0090, 0x41  },
        { 0x000A, 0x012 },
        { 0x000B, 0x123 },
        { 0x000C, 0x233 },
        { 0x000D, 0x114 },
        { 0x0E9E, 0x1   },
        { 0x0EA1, 0x1   },
        { 0x0F07, 0x1   },
        { 0x0F0A, 0x1   },
        { 0x0F15, 0x1   },
        { 0x0F18, 0x1   },
        { 0x0F1E, 0x1   },
        { 0x0F29, 0x1   },
        { 0x0F33, 0x1   },
        { 0x0F55, 0x1   },
        { 0x0F65, 0x1   }
    };

    for(const auto& op_code : expected)
    {
        const chip::OpCode result = chip::decode(program, PC);

        ASSERT_EQ(op_code.code, result.code);
        ASSERT_EQ(op_code.data, result.data);
        PC += 2;
    }
}

TEST(CPUTest, CanExecute0xE0)
{
    chip::OpCode op_code{};
    op_code.code = 0xE0;
    op_code.data = 0x0;

    chip::CPU cpu{};

    for(int i = 0 ; i < cpu.screen.size() ; i++) cpu.screen[i] = 1;

    op_code_0xE0(cpu, op_code);

    for(int i = 0 ; i < cpu.screen.size() ; i++) ASSERT_EQ(cpu.screen[i], 0);
}

TEST(CPUTest, CanExecute0x2)
{
    chip::OpCode subroutine1{};
    subroutine1.code = 0x2;
    subroutine1.data = 0x322;

    chip::OpCode subroutine2{};
    subroutine2.code = 0x2;
    subroutine2.data = 0x123;

    chip::CPU cpu{};
    cpu.PC = 0x80;

    op_code_0x2(cpu, subroutine1);

    ASSERT_EQ(cpu.PC, 0x322);
    ASSERT_EQ(cpu.SP, 0x1);
    ASSERT_EQ(cpu.stack[cpu.SP - 1], 0x80);    

    op_code_0x2(cpu, subroutine2);

    ASSERT_EQ(cpu.PC, 0x123);
    ASSERT_EQ(cpu.SP, 0x2);
    ASSERT_EQ(cpu.stack[cpu.SP - 1], 0x322); 
}

TEST(CPUTest, CanExecute0xEE)
{
    chip::OpCode op_code{};
    op_code.code = 0xEE;
    op_code.data = 0x0;
    
    chip::OpCode subroutine1{};
    subroutine1.code = 0x2;
    subroutine1.data = 0x322;

    chip::OpCode subroutine2{};
    subroutine2.code = 0x2;
    subroutine2.data = 0x123;

    chip::CPU cpu{};
    cpu.PC = 0x80;

    op_code_0x2(cpu, subroutine1);
    op_code_0x2(cpu, subroutine2);
    
    op_code_0xEE(cpu, op_code);

    ASSERT_EQ(cpu.PC, 0x322);
    ASSERT_EQ(cpu.SP, 0x1);
    
    op_code_0xEE(cpu, op_code);

    ASSERT_EQ(cpu.PC, 0x80);
    ASSERT_EQ(cpu.SP, 0x0);
}

TEST(CPUTest, CanExecute0x1)
{
    chip::OpCode op_code{};
    op_code.code = 0x1;
    op_code.data = 0x223;

    chip::CPU cpu{};

    op_code_0x1(cpu, op_code);

    ASSERT_EQ(cpu.PC, 0x223);
}

TEST(CPUTest, CanExecute0x3)
{
    chip::OpCode op_code{};
    op_code.code = 0x3;
    op_code.data = 0x11;

    chip::CPU cpu{};
    cpu.PC   = 0x80;
    cpu.V[0] = 0x11;
    
    op_code_0x3(cpu, op_code); 

    ASSERT_EQ(cpu.PC, 0x82);   
}

TEST(CPUTest, CanExecute0x4)
{
    chip::OpCode op_code{};
    op_code.code = 0x4;
    op_code.data = 0x132;

    chip::CPU cpu{};
    cpu.PC   = 0x80;
    cpu.V[1] = 0x23;

    op_code_0x4(cpu, op_code);  
    ASSERT_EQ(cpu.PC, 0x82);   
}

TEST(CPUTest, CanExecute0x50)
{
    chip::OpCode op_code{};
    op_code.code = 0x50;
    op_code.data = 0x21;

    chip::CPU cpu{};
    cpu.PC   = 0x80; 
    cpu.V[2] = 0x12;
    cpu.V[1] = 0x12;

    op_code_0x50(cpu, op_code);  
    ASSERT_EQ(cpu.PC, 0x82);  
}

TEST(CPUTest, CanExecute0x6)
{
    chip::OpCode op_code{};
    op_code.code = 0x6;
    op_code.data = 0x311;

    chip::CPU cpu{};
    
    op_code_0x6(cpu, op_code);
    ASSERT_EQ(cpu.V[3], 0x11);    
}

TEST(CPUTest, CanExecute0x7)
{
    chip::OpCode op_code{};
    op_code.code = 0x7;
    op_code.data = 0x40F;

    chip::CPU cpu{};
    cpu.V[4] = 0x5;

    op_code_0x7(cpu, op_code);
    ASSERT_EQ(cpu.V[4], 0x14);
}

TEST(CPUTest, CanExecute0x80)
{
    chip::OpCode op_code{};
    op_code.code = 0x80;
    op_code.data = 0x51;

    chip::CPU cpu{};
    cpu.V[1] = 0xA;

    op_code_0x80(cpu, op_code); 
    ASSERT_EQ(cpu.V[5], 0xA);   
}

TEST(CPUTest, CanExecute0x81)
{
    chip::OpCode op_code{};
    op_code.code = 0x81;
    op_code.data = 0x61;

    chip::CPU cpu{};
    cpu.V[6] = 0xA;
    cpu.V[1] = 0xF;

    op_code_0x81(cpu, op_code);
    ASSERT_EQ(cpu.V[6], 0xA | 0xF); 
}

TEST(CPUTest, CanExecute0x82)
{
    chip::OpCode op_code{};
    op_code.code = 0x82;
    op_code.data = 0x71;

    chip::CPU cpu{};
    cpu.V[7] = 0xA;
    cpu.V[1] = 0xF;

    op_code_0x82(cpu, op_code);
    ASSERT_EQ(cpu.V[7], 0xA & 0xF);    
}

TEST(CPUTest, CanExecute0x83)
{
    chip::OpCode op_code{};
    op_code.code = 0x83;
    op_code.data = 0x81;

    chip::CPU cpu{};
    cpu.V[8] = 0xA;
    cpu.V[1] = 0xF;

    op_code_0x83(cpu, op_code);
    ASSERT_EQ(cpu.V[8], 0xA ^ 0xF);  
}

TEST(CPUTest, CanExecute0x84)
{
    chip::OpCode op_code{};
    op_code.code = 0x84;
    op_code.data = 0x91;

    chip::CPU cpu{};
    cpu.V[9] = 0x1;
    cpu.V[1] = 0x1;

    op_code_0x84(cpu, op_code);

    ASSERT_EQ(cpu.V[9], 0x2);
    ASSERT_EQ(cpu.V[0xF], 0x0);
    
    cpu.V[9] = 0xFE;
    cpu.V[1] = 0x3;

    op_code_0x84(cpu, op_code);

    ASSERT_EQ(cpu.V[9], 0x1);
    ASSERT_EQ(cpu.V[0xF], 0x1);
}

TEST(CPUTest, CanExecute0x85)
{
    chip::OpCode op_code{};
    op_code.code = 0x85;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.V[0] = 0x4;
    cpu.V[1] = 0x5;

    op_code_0x85(cpu, op_code);
    
    ASSERT_EQ(cpu.V[0], (uint8_t)(0x4 - 0x5));
    ASSERT_EQ(cpu.V[0xF], 0x0);  

    cpu.V[0] = 0x5;
    cpu.V[1] = 0x4;

    op_code_0x85(cpu, op_code);
    ASSERT_EQ(cpu.V[0], (uint8_t)(0x5 - 0x4));
    ASSERT_EQ(cpu.V[0xF], 0x1);  
}

TEST(CPUTest, CanExecute0x86)
{
    chip::OpCode op_code{};
    op_code.code = 0x86;
    op_code.data = 0x11;

    chip::CPU cpu{};
    cpu.V[1] = 0xD;

    op_code_0x86(cpu, op_code);
    ASSERT_EQ(cpu.V[1], 0xD >> 1);
    ASSERT_EQ(cpu.V[0xF], 0xD & 0x1);    
}   

TEST(CPUTest, CanExecute0x87)
{
    chip::OpCode op_code{};
    op_code.code = 0x87;
    op_code.data = 0x21;

    chip::CPU cpu{};
    cpu.V[2] = 0x4;
    cpu.V[1] = 0x5;

    op_code_0x87(cpu, op_code); 
    
    ASSERT_EQ(cpu.V[2], (uint8_t)(0x5 - 0x4));
    ASSERT_EQ(cpu.V[0xF], 0x1);  

    cpu.V[2] = 0x5;
    cpu.V[1] = 0x4;

    op_code_0x87(cpu, op_code); 

    ASSERT_EQ(cpu.V[2], (uint8_t)(0x4 - 0x5));
    ASSERT_EQ(cpu.V[0xF], 0x0);     
}

TEST(CPUTest, CanExecute0x8E)
{
    chip::OpCode op_code{};
    op_code.code = 0x8E;
    op_code.data = 0x31;

    chip::CPU cpu{};
    cpu.V[3] = 0xA1;

    op_code_0x8E(cpu, op_code);
    ASSERT_EQ(cpu.V[3], (uint8_t)(0xA1 << 1));
    ASSERT_EQ(cpu.V[0xF], 0xA1 & (0x1 << 8));
}

TEST(CPUTest, CanExecute0x90)
{
    chip::OpCode op_code{};
    op_code.code = 0x90;
    op_code.data = 0x41;

    chip::CPU cpu{};
    cpu.PC = 0xA;
    cpu.V[4] = 0x1;
    cpu.V[1] = 0x2;

    op_code_0x90(cpu, op_code);   
    ASSERT_EQ(cpu.PC, 0xC); 
}

TEST(CPUTest, CanExecute0xA)
{
    chip::OpCode op_code{};
    op_code.code = 0xA;
    op_code.data = 0x12;

    chip::CPU cpu{};

    op_code_0xA(cpu, op_code); 
    ASSERT_EQ(cpu.I, 0x12);   
}

TEST(CPUTest, CanExecute0xB)
{
    chip::OpCode op_code{};
    op_code.code = 0xB;
    op_code.data = 0x123;

    chip::CPU cpu{};
    cpu.V[0] = 0x5;

    op_code_0xB(cpu, op_code);
    ASSERT_EQ(cpu.PC, 0x123 + 0x5);    
}
/*
TEST(CPUTest, CanExecute0xC)
{
    chip::OpCode op_code{};
    op_code.code = 0xC;
    op_code.data = 0x233;

    chip::CPU cpu{};
    uint8_t random = rand() % 256;

    op_code_0xC(cpu, op_code);
    ASSERT_EQ(cpu.V[2], 0x33 & random);
}
*/
TEST(CPUTest, CanExecute0xD)
{
    chip::OpCode op_code{};
    op_code.code = 0xD;
    op_code.data = 0x15;

    chip::CPU cpu{};
    cpu.V[0] = 0x8;
    cpu.V[1] = 0x0;
    cpu.I    = 0x2;

    cpu.V[2] = 0xF0;
    cpu.V[3] = 0x90;
    cpu.V[4] = 0x90;
    cpu.V[5] = 0x90;
    cpu.V[6] = 0xF0;

    op_code_0xD(cpu, op_code);

    ASSERT_EQ(cpu.screen[8],  0x1); ASSERT_EQ(cpu.screen[9],  0x1);
    ASSERT_EQ(cpu.screen[10], 0x1); ASSERT_EQ(cpu.screen[11], 0x1);
    ASSERT_EQ(cpu.screen[12], 0x0); ASSERT_EQ(cpu.screen[13], 0x0);
    ASSERT_EQ(cpu.screen[14], 0x0); ASSERT_EQ(cpu.screen[15], 0x0);

    ASSERT_EQ(cpu.screen[72], 0x1); ASSERT_EQ(cpu.screen[73], 0x0);
    ASSERT_EQ(cpu.screen[74], 0x0); ASSERT_EQ(cpu.screen[75], 0x1);
    ASSERT_EQ(cpu.screen[76], 0x0); ASSERT_EQ(cpu.screen[77], 0x0);
    ASSERT_EQ(cpu.screen[78], 0x0); ASSERT_EQ(cpu.screen[79], 0x0);

    ASSERT_EQ(cpu.screen[136], 0x1); ASSERT_EQ(cpu.screen[137], 0x0);
    ASSERT_EQ(cpu.screen[138], 0x0); ASSERT_EQ(cpu.screen[139], 0x1);
    ASSERT_EQ(cpu.screen[140], 0x0); ASSERT_EQ(cpu.screen[141], 0x0);
    ASSERT_EQ(cpu.screen[142], 0x0); ASSERT_EQ(cpu.screen[143], 0x0);
    
    ASSERT_EQ(cpu.screen[200], 0x1); ASSERT_EQ(cpu.screen[201], 0x0);
    ASSERT_EQ(cpu.screen[202], 0x0); ASSERT_EQ(cpu.screen[203], 0x1);
    ASSERT_EQ(cpu.screen[204], 0x0); ASSERT_EQ(cpu.screen[205], 0x0);
    ASSERT_EQ(cpu.screen[206], 0x0); ASSERT_EQ(cpu.screen[207], 0x0);

    ASSERT_EQ(cpu.screen[264], 0x1); ASSERT_EQ(cpu.screen[265], 0x1);
    ASSERT_EQ(cpu.screen[266], 0x1); ASSERT_EQ(cpu.screen[267], 0x1);
    ASSERT_EQ(cpu.screen[268], 0x0); ASSERT_EQ(cpu.screen[269], 0x0);
    ASSERT_EQ(cpu.screen[270], 0x0); ASSERT_EQ(cpu.screen[271], 0x0);
}

TEST(CPUTest, CanExecute0xE9E)
{
    chip::OpCode test_a{};
    test_a.code = 0xE9E;
    test_a.data = 0x1;

    chip::OpCode test_b{};
    test_b.code = 0xE9E;
    test_b.data = 0x2;

    chip::CPU cpu{};
    cpu.PC = 0; 

    cpu.V[0x1] = 0xA;
    cpu.key_pad |= (0x1 << 0xA);
    
    cpu.V[0x2] = 0x1;
    cpu.key_pad|= (0x0 << 0x1);

    op_code_0xE9E(cpu, test_a);
    ASSERT_EQ(cpu.PC, 0x2);

    op_code_0xE9E(cpu, test_b);
    ASSERT_EQ(cpu.PC, 0x2);
}

TEST(CPUTest, CanExecute0xEA1)
{
    chip::OpCode test_a{};
    test_a.code = 0xEA1;
    test_a.data = 0x1;

    chip::OpCode test_b{};
    test_b.code = 0xEA1;
    test_b.data = 0x2;

    chip::CPU cpu{};
    cpu.PC = 0; 

    cpu.V[0x1] = 0xA;
    cpu.key_pad |= (0x1 << 0xA);
    
    cpu.V[0x2] = 0x1;
    cpu.key_pad|= (0x0 << 0x1);

    op_code_0xEA1(cpu, test_a);
    ASSERT_EQ(cpu.PC, 0x0);

    op_code_0xEA1(cpu, test_b);
    ASSERT_EQ(cpu.PC, 0x2);
}

TEST(CPUTest, CanExecute0xF07)
{
    chip::OpCode op_code{};
    op_code.code = 0xF07;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.DT = 0x55;

    op_code_0xF07(cpu, op_code);
    ASSERT_EQ(cpu.V[1], 0x55);     
}

TEST(CPUTest, CanExecute0xF0A)
{
    chip::OpCode op_code{};
    op_code.code = 0xF0A;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.PC = 2;

    op_code_0xF0A(cpu, op_code);
    ASSERT_EQ(cpu.PC, 0x0);

    cpu.key_pad |= (0x1 << 0xA);
    cpu.PC += 2;

    op_code_0xF0A(cpu, op_code);
    ASSERT_EQ(cpu.PC, 0x2);
    ASSERT_EQ(cpu.V[0x1], 0xA);
    
}

TEST(CPUTest, CanExecute0xF15)
{
    chip::OpCode op_code{};
    op_code.code = 0xF15;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.V[1] = 0x55;

    op_code_0xF15(cpu, op_code);
    ASSERT_EQ(cpu.DT, cpu.V[1]);
} 

TEST(CPUTest, CanExecute0xF18)
{
    chip::OpCode op_code{};
    op_code.code = 0xF18;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.V[1] = 0x42;

    op_code_0xF18(cpu, op_code);
    ASSERT_EQ(cpu.ST, cpu.V[1]);    
}

TEST(CPUTest, CanExecute0xF1E)
{
    chip::OpCode op_code{};
    op_code.code = 0xF1E;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.I = 0x22;
    cpu.V[1] = 0x22;

    op_code_0xF1E(cpu, op_code); 
    ASSERT_EQ(cpu.I, 0x22 + 0x22);   
}

TEST(CPUTest, CanExecute0xF29)
{
    chip::OpCode op_code_0{0xF29, 0x0};
    chip::OpCode op_code_5{0xF29, 0x5};
    chip::OpCode op_code_A{0xF29, 0xA};
    chip::OpCode op_code_F{0xF29, 0xF};

    chip::CPU cpu{};
    cpu.V[0x0] = 0x0;
    cpu.V[0x5] = 0x5;
    cpu.V[0xA] = 0xA;
    cpu.V[0xF] = 0xF;

    op_code_0xF29(cpu, op_code_0);
    ASSERT_EQ(cpu.I, 0x0);

    op_code_0xF29(cpu, op_code_5);
    ASSERT_EQ(cpu.I, 0x19);

    op_code_0xF29(cpu, op_code_A);
    ASSERT_EQ(cpu.I, 0x32);

    op_code_0xF29(cpu, op_code_F);
    ASSERT_EQ(cpu.I, 0x4B);
}

TEST(CPUTest, CanExecute0xF33)
{
    chip::OpCode op_code{};
    op_code.code = 0xF33;
    op_code.data = 0x1;

    chip::CPU cpu{};
    cpu.I = 0x0;
    cpu.V[0x1] = 254;

    op_code_0xF33(cpu, op_code);
    ASSERT_EQ(cpu.memory[0x0], 0x2);
    ASSERT_EQ(cpu.memory[0x1], 0x5);
    ASSERT_EQ(cpu.memory[0x2], 0x4);
}

TEST(CPUTest, CanExecute0xF55)
{
    chip::OpCode op_code{};
    op_code.code = 0xF55;
    op_code.data = 0x4;

    chip::CPU cpu{};
    cpu.I = 0x0;
    cpu.V[0x0] = 0xA;
    cpu.V[0x1] = 0xB;
    cpu.V[0x2] = 0xC;
    cpu.V[0x3] = 0xD;
    cpu.V[0x4] = 0xE;

    op_code_0xF55(cpu, op_code);
    ASSERT_EQ(cpu.memory[0x0], 0xA);
    ASSERT_EQ(cpu.memory[0x1], 0xB);
    ASSERT_EQ(cpu.memory[0x2], 0xC);
    ASSERT_EQ(cpu.memory[0x3], 0xD);
    ASSERT_EQ(cpu.memory[0x4], 0xE);
}

TEST(CPUTest, CanExecute0xF65)
{
    chip::OpCode op_code{};
    op_code.code = 0xF65;
    op_code.data = 0x4;

    chip::CPU cpu{};
    cpu.I = 0x0;
    cpu.memory[0x0] = 0xA;
    cpu.memory[0x1] = 0xB;
    cpu.memory[0x2] = 0xC;
    cpu.memory[0x3] = 0xD;
    cpu.memory[0x4] = 0xE;

    op_code_0xF65(cpu, op_code);
    ASSERT_EQ(cpu.V[0x0], 0xA);
    ASSERT_EQ(cpu.V[0x1], 0xB);
    ASSERT_EQ(cpu.V[0x2], 0xC);
    ASSERT_EQ(cpu.V[0x3], 0xD);
    ASSERT_EQ(cpu.V[0x4], 0xE); 
}

TEST(CPUTest, CanLoadROM)
{
    chip::CPU cpu{};

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

    chip::load_ROM(cpu, "../resources/ROMS/pong.rom");

    for(int i = 0; i < program.size() ; i++) 
        ASSERT_EQ(cpu.memory[chip::ROM_START + i], program[i]);
}