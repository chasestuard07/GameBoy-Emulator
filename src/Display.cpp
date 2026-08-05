#include "Display.h"

Display::Display()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Game Boy Emulator", //Title
        SDL_WINDOWPOS_CENTERED, //X Position
        SDL_WINDOWPOS_CENTERED, //Y Position
        640, //Width
        576, //Height
        0 //Flags
    );
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        160,
        144
    );
}

Display::~Display()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Display::Render(uint8_t* framebuffer)
{
    for(int i = 0; i < 160 * 144; i++)
    {
        uint8_t shade = framebuffer[i];
        uint8_t color; 
        switch (shade)
        {
            case 0: color = 255; break;
            case 1: color = 170; break;
            case 2: color = 85; break;
            case 3: color = 0; break;
        }

        screen[i * 3]     = color;
        screen[i * 3 + 1] = color;
        screen[i * 3 + 2] = color;
    }

    SDL_UpdateTexture(
        texture,
        nullptr,
        screen,
        160 * 3
    );
    SDL_RenderClear(renderer);
    SDL_RenderCopy(
        renderer,
        texture,
        nullptr,
        nullptr
    );
    SDL_RenderPresent(renderer);
}