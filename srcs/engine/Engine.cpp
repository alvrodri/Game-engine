#include "engine.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <utility>

const float scale_factor = 480;

Mesh mesh;
std::vector<Triangle> trianglesToRender;

static void load_obj_file_data(std::string filename) {
  FILE *file;
  file = fopen(filename.c_str(), "r");
  char line[1024];

  while (fgets(line, 1024, file)) {
    if (strncmp(line, "v ", 2) == 0) {
      Vec3 vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      mesh.vertices.push_back(vertex);
    }

    if (strncmp(line, "f ", 2) == 0) {
      int vertex_indices[3];
      int texture_indices[3];
      int normal_indices[3];

      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex_indices[0],
             &texture_indices[0], &normal_indices[0], &vertex_indices[1],
             &texture_indices[1], &normal_indices[1], &vertex_indices[2],
             &texture_indices[2], &normal_indices[2]);

      Face face = {.a = vertex_indices[0],
                   .b = vertex_indices[1],
                   .c = vertex_indices[2]};
      mesh.faces.push_back(face);
    }
  }
}

Engine::Engine() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Couldn't initialize SDL (" << strerror(errno) << ")"
              << std::endl;
    return;
  }

  this->_window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!this->_window) {
    std::cerr << "Couldn't create window (" << strerror(errno) << ")"
              << std::endl;
    return;
  }

  this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
  if (!this->_renderer) {
    std::cerr << "Couldn't create renderer (" << strerror(errno) << ")"
              << std::endl;
    return;
  }

  this->_colorBuffer = std::make_unique<ColorBuffer>(WIDTH, HEIGHT);
  this->_colorBuffer->setTexture(
      SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT));

  this->_cameraPosition = Vec3(0, 0, -20);
  this->_previousFrametime = 0;
  this->_running = true;

  load_obj_file_data("./assets/cube.obj");
}

Engine::Engine(const Engine &other) {
  // TODO: Copy constructor
}

Engine::~Engine() {
  SDL_DestroyTexture(this->_colorBuffer->getTexture());
  SDL_DestroyRenderer(this->_renderer);
  SDL_DestroyWindow(this->_window);
  SDL_Quit();
}

Engine &Engine::operator=(const Engine &other) {
  // TODO: Copy assignment
  return *this;
}

void Engine::loop() {
  while (this->_running) {
    this->processInput();
    this->update();
    this->render();
  }
}

bool pressed = false;
float zoom = 30.0f;
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
    if (event.key.keysym.sym == SDLK_UP)
      mesh.translation.y -= 0.5;
    if (event.key.keysym.sym == SDLK_DOWN)
      mesh.translation.y += 0.5;
    if (event.key.keysym.sym == SDLK_LEFT)
      mesh.translation.x -= 0.5;
    if (event.key.keysym.sym == SDLK_RIGHT)
      mesh.translation.x += 0.5;
    if (event.key.keysym.sym == SDLK_PLUS)
      mesh.scale.x += 0.1, mesh.scale.y += 0.1;
    if (event.key.keysym.sym == SDLK_MINUS)
      mesh.scale.x -= 0.1, mesh.scale.y -= 0.1;
    break;
  case SDL_MOUSEMOTION:
    if (pressed) {
      mesh.rotation.y += (event.motion.xrel * 0.01);
      mesh.rotation.x += (event.motion.yrel * 0.01);
    }
    break;
  case SDL_MOUSEWHEEL:
    zoom += event.wheel.y * 0.25;
    break;
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    pressed = !pressed;
    break;
  }
}

void Engine::update() {
  int timeToWait =
      FRAME_TARGET_TIME - (SDL_GetTicks() - this->_previousFrametime);
  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
    SDL_Delay(timeToWait);
  }
  this->_previousFrametime = SDL_GetTicks();

  for (int i = 0; i < mesh.faces.size(); i++) {
    Face mesh_face = mesh.faces[i];

    Vec3 face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[mesh_face.c - 1];

    std::vector<Vec3> transformedVertexes;
    for (int j = 0; j < 3; j++) {
      face_vertices[j].rotateX(mesh.rotation.x);
      face_vertices[j].rotateY(mesh.rotation.y);
      face_vertices[j].rotateZ(mesh.rotation.z);

      Vec4 transformedVertex = Vec4(face_vertices[j]);

      transformedVertex = transformedVertex * getScaleMatrix(mesh.scale).m;
      transformedVertex = transformedVertex * getTranslateMatrix(mesh.translation).m;

      transformedVertex.z += zoom;

      transformedVertexes.push_back(
          Vec3(transformedVertex.x, transformedVertex.y, transformedVertex.z));
    }

    Vec3 vA = transformedVertexes[0];
    Vec3 vB = transformedVertexes[1];
    Vec3 vC = transformedVertexes[2];

    Vec3 vAB = vB - vA;
    vAB.normalize();

    Vec3 vAC = vC - vA;
    vAC.normalize();

    Vec3 nV = vAB.crossProduct(vAC);
    nV.normalize();

    Vec3 cV = this->_cameraPosition - vA;
    cV.normalize();

    if (nV.dotProduct(cV) < 0)
      continue;

    Triangle projectedTriangle;
    for (int j = 0; j < 3; j++) {
      Vec2 projected = this->project(transformedVertexes[j]);

      projected.x += WIDTH / 2;
      projected.y += HEIGHT / 2;

      projectedTriangle.points[j] = projected;
    }
    projectedTriangle.avgZ =
        (transformedVertexes[0].z + transformedVertexes[1].z +
         transformedVertexes[2].z) /
        3;

    trianglesToRender.push_back(projectedTriangle);
  }

  std::sort(trianglesToRender.begin(), trianglesToRender.end(), TriangleSort);
}

void Engine::render() {
  this->_colorBuffer->drawGrid();
  for (int i = 0; i < trianglesToRender.size(); i++) {
    Triangle triangle = trianglesToRender[i];

    this->_colorBuffer->drawFilledTriangle(triangle, 0xFFFF0000);
    this->_colorBuffer->drawWireframe(triangle, 0x00000000);
  }

  this->_colorBuffer->render(this->_renderer);
  this->_colorBuffer->clear(0xFF000000);
  trianglesToRender.clear();

  SDL_RenderPresent(this->_renderer);
}

Vec2 Engine::project(Vec3 &point) {
  return Vec2((point.x * scale_factor) / point.z,
              (point.y * scale_factor) / point.z);
}
