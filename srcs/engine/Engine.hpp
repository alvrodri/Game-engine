#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "../color/ColorBuffer.hpp"

#define WIDTH 640
#define HEIGHT 480

class Engine {
    public:
    Engine();
    Engine(const Engine& other);

    ~Engine();

    Engine& operator=(const Engine& other);

    void    loop();
    void    processInput();
    void    render();

    void    clearImage(const uint32_t color);

    private:
    bool                            _running;
    SDL_Window*                     _window;
    SDL_Renderer*                   _renderer;
    std::unique_ptr<ColorBuffer>    _colorBuffer;
};