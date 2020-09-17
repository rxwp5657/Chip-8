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