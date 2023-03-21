#include "Engine.hpp"

const float fov_factor = 640;

typedef struct {
    Vector2 points[3];
} triangle_t;

triangle_t trianglesToRender[12];

Vector3 mesh_vertices[8] = {
    Vector3(-1, -1, -1),
    Vector3(-1, 1, -1),
    Vector3(1, 1, -1),
    Vector3(1, -1, -1),
    Vector3(1, 1, 1),
    Vector3(1, -1, 1),
    Vector3(-1, 1, 1),
    Vector3(-1, -1, 1)
};

typedef struct {
    int a, b, c;
} face_t;

face_t mesh_faces[12] = {
    {1, 2, 3},
    {1, 3, 4},

    {4, 3, 5},
    {4, 5, 6},

    {6, 5, 7},
    {6, 7, 8},

    {8, 7, 2},
    {8, 2, 1},

    {2, 7, 5},
    {2, 5, 3},

    {6, 8, 1},
    {6, 1, 4},
};

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
    this->_previousFrametime = 0;
    this->_running = true;
}

Engine::Engine(const Engine& other) {
    //TODO: Copy constructor
}

Engine::~Engine() {
    SDL_DestroyTexture(this->_colorBuffer->getTexture());
    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
    SDL_Quit();
}

Engine &Engine::operator=(const Engine& other) {
    //TODO: Copy assignment
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
            break;
    }
}

void    Engine::update() {
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - this->_previousFrametime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    
    this->_previousFrametime = SDL_GetTicks();

    for (int i = 0; i < 12; i++) {
        face_t mesh_face = mesh_faces[i];

        Vector3 face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle;
        for (int j = 0; j < 3; j++) {
            Vector3 transformed = face_vertices[j];
            
            transformed.z -= _cameraPosition.z;
            Vector2 projected = this->project(transformed);

            projected.x += WIDTH / 2;
            projected.y += HEIGHT / 2;

            projected_triangle.points[j] = projected;
        }

        trianglesToRender[i] = projected_triangle;
    }
}

void    Engine::render() {
    this->_colorBuffer->drawGrid();

    for (int i = 0; i < 12; i++) {
        triangle_t triangle = trianglesToRender[i];

        //std::cout << "(" << triangle.points[0].x << ", " << triangle.points[0].y << ")" << std::endl;
        //std::cout << "(" << triangle.points[1].x << ", " << triangle.points[1].y << ")" << std::endl;
        //std::cout << "(" << triangle.points[2].x << ", " << triangle.points[2].y << ")" << std::endl;

        this->_colorBuffer->drawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
        this->_colorBuffer->drawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
        this->_colorBuffer->drawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
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