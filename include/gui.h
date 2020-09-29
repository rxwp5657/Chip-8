#ifndef GUI_H
#define GUI_H

#include <cstdint>
#include "cpu.h"

namespace chip
{
    uint16_t key_pad = 0x0;
    uint8_t  screen[SCREEN_SIZE] = {};

    void press_button(uint16_t button)
    {
        key_pad |= (0x1 << button);
    }

    uint32_t screen_buf()
    {
        return (uint32_t)&screen[0];
    }

    uint32_t screen_size()
    {
        return chip::SCREEN_SIZE;
    }
}

#endif