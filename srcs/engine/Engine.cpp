#include "Engine.hpp"

const float fov_factor = 640;

typedef struct {
    Vector2 points[3];
} triangle_t;

mesh_t mesh = {
    .vertices = {},
    .faces = {},
};

std::vector<triangle_t> trianglesToRender;

static void load_obj_file_data(char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    char line[1024];

    while (fgets(line, 1024, file)) {
        if (strncmp(line, "v ", 2) == 0) {
            Vector3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            mesh.vertices.push_back(vertex);
        }

        if (strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0], 
                &vertex_indices[1], &texture_indices[1], &normal_indices[1], 
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            ); 
            face_t face = {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2]
            };
            mesh.faces.push_back(face);
        }
    }
}

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

    this->_cameraPosition = Vector3(0, 0, -20.0f);
    this->_previousFrametime = 0;
    this->_running = true;

    load_obj_file_data("formula.obj");
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

    mesh.rotation.z = 3.15;
    mesh.rotation.y += 0.01;
    for (int i = 0; i < mesh.faces.size(); i++) {
        face_t mesh_face = mesh.faces[i];

        Vector3 face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        triangle_t projected_triangle;
        for (int j = 0; j < 3; j++) {
            Vector3 transformed = face_vertices[j];
            
            transformed.rotateX(mesh.rotation.x);
            transformed.rotateY(mesh.rotation.y);
            transformed.rotateZ(mesh.rotation.z);

            transformed.z -= _cameraPosition.z;

            Vector2 projected = this->project(transformed);

            projected.x += WIDTH / 2;
            projected.y += HEIGHT / 2;

            projected_triangle.points[j] = projected;
        }

        trianglesToRender.push_back(projected_triangle);
    }
}

void    Engine::render() {
    this->_colorBuffer->drawGrid();

    for (int i = 0; i < trianglesToRender.size(); i++) {
        triangle_t triangle = trianglesToRender[i];

        this->_colorBuffer->drawLine(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, 0xFFF0FF0F);
        this->_colorBuffer->drawLine(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xFFF0FF0F);
        this->_colorBuffer->drawLine(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, 0xFFF0FF0F);
    }

    this->_colorBuffer->render(this->_renderer);

    trianglesToRender.clear();
    this->_colorBuffer->clear(0xFF000000);

    SDL_RenderPresent(this->_renderer);
}

Vector2 Engine::project(Vector3& point) {
    return Vector2(
         (point.x * fov_factor) / point.z,
         (point.y * fov_factor) / point.z
    );
}