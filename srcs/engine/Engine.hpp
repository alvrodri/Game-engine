#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include "../color/colorbuffer.hpp"
#include "../vector/vector.hpp"
#include "../mesh/mesh.hpp"
#include "../matrix/matrix.hpp"

#define WIDTH 800
#define HEIGHT 600

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

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

    void    importMesh(const std::string& path);

    Vec2 project(Vec3& point);

    private:
    bool                            _running;
    SDL_Window*                     _window;
    SDL_Renderer*                   _renderer;
    std::unique_ptr<ColorBuffer>    _colorBuffer;
    Vec3                            _cameraPosition;
    long                            _previousFrametime;
    std::vector<Mesh>               _meshes;
    std::vector<Triangle>           _toRender;
};
