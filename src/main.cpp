#include <array>
#include <SDL.h>
#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <iostream>

#include "../include/cpu.h"
#include "../include/gui.h"
#include "../include/debbuger.h"
#include "../include/disassembler.h"

int main(int argc, char **argv)
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) 
    {
        std::cout << "Couldn't initialize SDL because: " << SDL_GetError();
        return 1;
    }

    const uint16_t WIDTH  = 800;
    const uint16_t HEIGHT = 600;

    SDL_Window*   window   = chip::make_window(WIDTH, HEIGHT);
    SDL_Renderer* renderer = chip::make_renderer(window, WIDTH, HEIGHT);
    SDL_Texture*  texture  = chip::make_texture(renderer, SDL_PIXELFORMAT_ARGB8888, chip::SCREEN_WIDHT, chip::SCREEN_HEIGHT);
    
    std::array<SDL_KeyCode, 16> key_codes =
    {{
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v
    }};

    uint32_t back_buffer[chip::SCREEN_WIDHT * chip::SCREEN_HEIGHT] = {};

    chip::CPU chip8{};
    chip::load_font_set(chip8);

    chip::load_ROM(chip8, "../resources/ROMS/GUESS");
    
    for (;;)
    {
       chip::cycle(chip8);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) return 0;
           
            if (event.type == SDL_KEYDOWN) 
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) return 0;

                for (int i = 0; i < 16; ++i) 
                {
                    if (event.key.keysym.sym == key_codes[i]) 
                    {
                        chip8.key_pad |= (0x1 << i);
                    }
                }
            }

            if (event.type == SDL_KEYUP) 
            {
                for (int i = 0; i < 16; ++i) 
                {
                    if (event.key.keysym.sym == key_codes[i]) 
                    {
                        chip8.key_pad &= ~(0x1 << i);
                    }
                }
            }
        }

        if(chip8.draw)
        {
            chip8.draw = false;
            for (uint32_t i = 0 ; i < chip8.screen.size() ; i++)
            {
                uint8_t pixel   = chip8.screen[i];
                back_buffer[i]  = (0x00FFFFFF * pixel) | 0xFF000000;
            }
            SDL_UpdateTexture(texture, nullptr, back_buffer, 64 * sizeof(Uint32));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }

    return 0;
}