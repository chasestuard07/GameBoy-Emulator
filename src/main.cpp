#include <iostream>
#include "Emulator.h"
#include <filesystem>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Display.h"
#include "FileDialog.h"
#include <nfd.h>


int main()
{
    NFD_Init();
    Emulator emulator;
    std::string romPath;

    if (!OpenROMDialog(romPath))
    {
       std::cout << "No ROM selected\n";
        return 0;
    }

    if (!emulator.setROM(romPath))
    {
        return 1;
    }

    Display display;
    Joypad* joypad = emulator.GetJoypad();

    bool running = true;
    bool paused = false;
    const int frameDelay = 1000 / 60;


    while (running)
    {
        uint32_t frameStart = SDL_GetTicks();
        
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                        joypad->Press(RIGHT);
                        break;

                    case SDLK_LEFT:
                        joypad->Press(LEFT);
                        break;

                    case SDLK_UP:
                        joypad->Press(UP);
                        break;

                    case SDLK_DOWN:
                        joypad->Press(DOWN);
                        break;

                    case SDLK_z:
                        joypad->Press(A);
                        break;

                    case SDLK_x:
                        joypad->Press(B);
                        break;

                    case SDLK_RETURN:
                        joypad->Press(START);
                        break;

                    case SDLK_BACKSPACE:
                        joypad->Press(SELECT);
                        break;
                    
                    case SDLK_p:
                        paused = !paused;
                        break;
                    case SDLK_r:
                        emulator.Reset();
                        break;
                }
            }
            if(event.type == SDL_KEYUP)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                        joypad->Release(RIGHT);
                        break;

                    case SDLK_LEFT:
                        joypad->Release(LEFT);
                        break;

                    case SDLK_UP:
                        joypad->Release(UP);
                        break;

                    case SDLK_DOWN:
                        joypad->Release(DOWN);
                        break;

                    case SDLK_z:
                        joypad->Release(A);
                        break;

                    case SDLK_x:
                        joypad->Release(B);
                        break;

                    case SDLK_RETURN:
                        joypad->Release(START);
                        break;

                    case SDLK_BACKSPACE:
                        joypad->Release(SELECT);
                        break;
                }
            }
        }

        if(!paused) 
        {
            while(!emulator.FrameReady())
            {
                emulator.Step();
            }


            display.Render(emulator.GetFrameBuffer());
            emulator.ClearFrameReady();
            
            uint32_t frameTime = SDL_GetTicks() - frameStart;

            if(frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }

    NFD_Quit();
    return 0;
}