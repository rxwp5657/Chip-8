#include "../include/cpu.h"
#include "../include/gui.h"
#include <string>

#ifdef WASM
#include <emscripten/bind.h>

void start_emulation(const std::string& rom_name)
{
    chip::CPU cpu{};
    cpu.screen = &chip::screen[0];
    
    chip::load_font_set(cpu);
    chip::load_ROM(cpu, rom_name);

    for(;;)
    {
        chip::cycle(cpu);
    }
}

EMSCRIPTEN_BINDINGS(cpu) 
{
    emscripten::function("pressButton",&chip::press_button);
    emscripten::function("screenBuf",  &chip::screen_buf);
    emscripten::function("screenSize", &chip::screen_size);
    emscripten::function("startEmulation", &start_emulation);
}
#endif

int main(int argc, char **argv)
{
    /*
    chip::CPU cpu{};
    uint8_t screen[chip::SCREEN_SIZE] = {};

    cpu.screen = &screen[0];
    
    chip::load_font_set(cpu);
    chip::load_ROM(cpu, "../resources/ROMS/pong.rom");

    for(;;)
    {
        chip::cycle(cpu);
    }
    */
    return 0;
}