#pragma once

#include <SDL2/SDL.h>
#include <iostream>

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

    private:
    SDL_Window*     _window;
    SDL_Renderer*   _renderer;
    bool            _running;
};