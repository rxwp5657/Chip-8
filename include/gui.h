#ifndef GUI_H
#define GUI_H

#include <string>
#include <cstdint>
#include <iostream>
#include <SDL.h>

namespace chip
{
    SDL_Window* make_window(uint16_t width, uint16_t height)
    {
        SDL_Window* window = SDL_CreateWindow("Chip 8", 
                              SDL_WINDOWPOS_UNDEFINED, 
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              SDL_WINDOW_SHOWN);
        
        if (window == NULL) 
            throw std::runtime_error{"Unable to create window becuase: " + std::string{SDL_GetError()}};
        
        return window;
    }

    SDL_Renderer* make_renderer(SDL_Window* window, uint16_t width, uint16_t height)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_RenderSetLogicalSize(renderer, width, height);

        return renderer;
    }

    SDL_Texture* make_texture(SDL_Renderer* renderer, SDL_PixelFormatEnum pixel_format, uint16_t width, uint16_t height)
    {
        SDL_Texture* texture = SDL_CreateTexture(renderer,
                                                 SDL_PIXELFORMAT_ARGB8888,
                                                 SDL_TEXTUREACCESS_STREAMING,
                                                 width, 
                                                 height);

        return texture;
    }
}

#endif