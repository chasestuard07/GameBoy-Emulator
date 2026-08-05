#pragma once

#include <SDL.h>
#include <cstdint>

class Display
{
public:
    Display();
    ~Display();

    void Render(uint8_t* framebuffer);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    uint8_t screen[160 * 144 * 3];
};