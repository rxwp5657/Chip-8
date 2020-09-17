#ifndef OPCODE_H
#define OPCODE_H

#include <tuple>
#include <array>
#include <cstdint>
#include <unordered_map>

namespace chip
{
    struct OpCode
    {
        uint16_t code;
        uint16_t data;
    };
}

#endif