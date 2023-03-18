#include "Engine.hpp"

Engine::Engine() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Couldn't initialize SDL (" << strerror(errno) << ")" << std::endl;
        return;
    }

    this->_window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!this->_window) {
        std::cerr << "Couldn't create window (" << strerror(errno) << ")" << std::endl;
        return;
    }

    this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
    if (!this->_renderer) {
        std::cerr << "Couldn't create renderer (" << strerror(errno) << ")" << std::endl;
        return;
    }

    this->_colorBuffer = std::make_unique<ColorBuffer>(WIDTH, HEIGHT);
    this->_colorBuffer->setTexture(SDL_CreateTexture(
        this->_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    ));

    this->_running = true;
}

Engine::Engine(const Engine& other) {

}

Engine::~Engine() {
    SDL_DestroyTexture(this->_colorBuffer->getTexture());
    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
    SDL_Quit();
}

Engine &Engine::operator = (const Engine& other) {
    return *this;
}

void Engine::loop() {
    while (this->_running) {
        this->processInput();
        this->render();
    }
}

void Engine::processInput() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            this->_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                this->_running = false;
            break;
    }
}

void    Engine::render() {
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->_renderer);

    this->_colorBuffer->drawGrid();
    this->_colorBuffer->render(this->_renderer);

    SDL_RenderPresent(this->_renderer);
}