#include <iostream>
#include "Emulator.h"
#include <filesystem>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Display.h"


int main()
{
    Emulator emulator("../tests/Tetris (JUE) (V1.1) [!].gb");
    Display display;

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        emulator.Step();

        if(emulator.FrameReady())
        {
            display.Render(emulator.GetFrameBuffer());
            emulator.ClearFrameReady();
        }    
    }
    return 0;
}