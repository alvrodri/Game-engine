#include "engine.hpp"
#include <iostream>
#include <ostream>
#include <thread>

const float scale_factor = 480;

void Engine::importMesh(const std::string& path) {
  Mesh mesh;
  FILE *file = fopen(path.c_str(), "r");
  char line[1024];

  while (fgets(line, 1024, file)) {
    if (strncmp(line, "v ", 2) == 0) {
      Vec3 vertex;

      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      mesh.vertices.push_back(vertex);
    }

    if (strncmp(line, "f ", 2) == 0) {
      int vertexIndex[3];
      int textureIndex[3];
      int normalIndex[3];

      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
        &vertexIndex[0], &textureIndex[0], &normalIndex[0],
        &vertexIndex[1], &textureIndex[1], &normalIndex[1],
        &vertexIndex[2], &textureIndex[2], &normalIndex[2]
      );

      Face face = {
        .indexes = { vertexIndex[0], vertexIndex[1], vertexIndex[2]}
      };
      
      mesh.faces.push_back(face);
    }
  }

  this->_meshes.push_back(mesh);

  fclose(file);
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

  this->_cameraPosition = Vec3(0, 0, 0);

  this->_previousFrametime = 0;
  this->_running = true;

  this->importMesh("./assets/cube.obj");
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

void Engine::update() {
  int timeToWait =
      FRAME_TARGET_TIME - (SDL_GetTicks() - this->_previousFrametime);
  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
    SDL_Delay(timeToWait);
  }

  this->_previousFrametime = SDL_GetTicks();

  for (auto& mesh : this->_meshes) {
      mesh.translation.z = 10;
      mesh.rotation.x += 0.01;
      mesh.rotation.y += 0.01;
      mesh.rotation.z += 0.01;
      for (int i = 0; i < mesh.faces.size(); i++) {
        Face mesh_face = mesh.faces[i];

        Vec3 face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.indexes[0] - 1];
        face_vertices[1] = mesh.vertices[mesh_face.indexes[1] - 1];
        face_vertices[2] = mesh.vertices[mesh_face.indexes[2] - 1];

        mat4_t worldMatrix = getIdentity();
        worldMatrix = multiplyMatrix(getScaleMatrix(mesh.scale), worldMatrix);
        worldMatrix = multiplyMatrix(getZRotationMatrix(mesh.rotation.z), worldMatrix);
        worldMatrix = multiplyMatrix(getYRotationMatrix(mesh.rotation.y), worldMatrix);
        worldMatrix = multiplyMatrix(getXRotationMatrix(mesh.rotation.x), worldMatrix);
        worldMatrix = multiplyMatrix(getTranslateMatrix(mesh.translation), worldMatrix);

        std::vector<Vec3> transformedVertexes;
        for (int j = 0; j < 3; j++) {
          Vec4 transformedVertex = Vec4(face_vertices[j]) * worldMatrix.m;
          transformedVertexes.push_back(Vec3(transformedVertex.x, transformedVertex.y, transformedVertex.z));
        }

        Vec3 vA = transformedVertexes[0];
        Vec3 vB = transformedVertexes[1];
        Vec3 vC = transformedVertexes[2];

        Vec3 vAB = vB - vA;
        Vec3 vAC = vC - vA;
        Vec3 nV = vAB.crossProduct(vAC);
        Vec3 cV = this->_cameraPosition - vA;

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

        this->_toRender.push_back(projectedTriangle);
      }
  }

  std::sort(this->_toRender.begin(), this->_toRender.end(), TriangleSort);
}

void Engine::render() {
  this->_colorBuffer->drawGrid();

  for (int i = 0; i < this->_toRender.size(); i++) {
    Triangle triangle = this->_toRender[i];

    this->_colorBuffer->drawFilledTriangle(triangle, 0xFFFF0000);
  }

  this->_colorBuffer->render(this->_renderer);
  this->_colorBuffer->clear(0xFF000000);
  this->_toRender.clear();

  SDL_RenderPresent(this->_renderer);
}

Vec2 Engine::project(Vec3 &point) {
  return Vec2((point.x * scale_factor) / point.z,
              (point.y * scale_factor) / point.z);
}
