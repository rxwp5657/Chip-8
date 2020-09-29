#ifndef GUI_H
#define GUI_H

#include <thread>
#include <chrono>
#include <cstdint>
#include <emscripten.h>
#include <emscripten/bind.h>


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

    EM_JS(void, draw_screen, (uint8_t* screen_buf), {
        let canvas  = document.getElementById("canvas");
        let context = canvas.getContext('2d');
        
        context.clearRect(0, 0, 64, 32);

        for(let x = 0; x < 64; x++)
        {   
            for(let y = 0; y < 32; y++)
            {
                if(HEAP8[(x * 32 + y)])
                {
                    context.fillStyle = '#000';
                    context.fillRect(x, y, 1, 1);
                }
            }
        }
    });

    void start_emulation(const std::string& rom_name)
    {
        chip::CPU cpu{};
        cpu.screen = &chip::screen[0];
        
        chip::load_font_set(cpu);
        chip::load_ROM(cpu, rom_name);

        for(int i = 0 ; i < 100; i++)
        {
            chip::cycle(cpu);
            if(cpu.draw)
            {
                cpu.draw = false;
                draw_screen(cpu.screen);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    EMSCRIPTEN_BINDINGS(cpu) 
    {
        emscripten::function("pressButton",&chip::press_button);
        emscripten::function("screenBuf",  &chip::screen_buf);
        emscripten::function("screenSize", &chip::screen_size);
        emscripten::function("startEmulation", &start_emulation);
    }
}

#endif