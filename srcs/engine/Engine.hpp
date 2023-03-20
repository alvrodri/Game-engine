#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "../color/ColorBuffer.hpp"
#include "../vector/Vector.hpp"

#define WIDTH 800
#define HEIGHT 600

class Engine {
    public:
    Engine();
    Engine(const Engine& other);

    ~Engine();

    Engine& operator=(const Engine& other);

    void    loop();
    void    update();
    void    processInput();
    void    render();

    Vector2 project(Vector3& point);

    private:
    bool                            _running;
    SDL_Window*                     _window;
    SDL_Renderer*                   _renderer;
    std::unique_ptr<ColorBuffer>    _colorBuffer;
    Vector3                         _cameraPosition;
};