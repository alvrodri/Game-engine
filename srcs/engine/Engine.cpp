#include "Engine.hpp"

const int N_POINTS = 9 * 9 * 9;
std::vector<Vector3> cubePoints;
std::vector<Vector2> projectedPoints(N_POINTS);
const float fov_factor = 640;
Vector3 rotation(0, 0, 0);

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

    this->_cameraPosition = Vector3(0, 0, -5.0f);

    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                cubePoints.push_back(Vector3(x, y, z));
            }
        }
    }

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
        this->update();
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
            else if (event.key.keysym.sym == SDLK_s)
                this->_cameraPosition.z -= 0.25;
            else if (event.key.keysym.sym == SDLK_w)
                this->_cameraPosition.z += 0.25;
            break;
    }
}

void    Engine::update() {
    rotation.x += 0.01;
    rotation.y += 0.01;
    rotation.z += 0.01;

    for (int i = 0; i < N_POINTS; i++) {
        Vector3 point = cubePoints[i];

        point.rotateX(rotation.x);
        point.rotateY(rotation.y);
        point.rotateZ(rotation.z);

        point.z -= this->_cameraPosition.z;

        Vector2 projectedPoint = this->project(point);
        projectedPoints[i] = projectedPoint;
    }
}

void    Engine::render() {
    this->_colorBuffer->drawGrid();

    for (int i = 0; i < N_POINTS; i++) {
        this->_colorBuffer->drawRect(projectedPoints[i].x + WIDTH / 2, projectedPoints[i].y + HEIGHT / 2, 4, 4, 0xFFFF0000);
    }

    this->_colorBuffer->render(this->_renderer);
    this->_colorBuffer->clear(0xFF000000);

    SDL_RenderPresent(this->_renderer);
}

Vector2 Engine::project(Vector3& point) {
    return Vector2(
         (point.x * fov_factor) / point.z,
         (point.y * fov_factor) / point.z
    );
}