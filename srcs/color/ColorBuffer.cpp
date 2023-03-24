#include "colorbuffer.hpp"

ColorBuffer::ColorBuffer() {
    this->_buffer = std::vector<uint32_t>();
}

ColorBuffer::ColorBuffer(const int width, const int height) {
    this->_width = width;
    this->_height = height;
    this->_buffer = std::vector<uint32_t>(width * height);
}

ColorBuffer::ColorBuffer(const ColorBuffer& other) {
    this->_buffer = other._buffer;
}

ColorBuffer::~ColorBuffer() {

}

ColorBuffer& ColorBuffer::operator=(const ColorBuffer& other) {
    this->_buffer = other._buffer;
    return *this;
}

void ColorBuffer::clear(uint32_t color) {
    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            this->draw(x, y, color);
        }
    }
}

void ColorBuffer::draw(int x, int y, uint32_t color) {
    if (x >= this->_width || x < 0 || y >= this->_height || y < 0)
        return;

    this->_buffer[(this->_width * y) + x] = color;
}

uint32_t ColorBuffer::getColor(int x, int y) {
    if (x >= this->_width || x < 0 || y >= this->_height || y < 0)
        return 0x00000000;

    return this->_buffer[(this->_width * y) + x];
}

void ColorBuffer::drawGrid() {
    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            if (y % 10 == 0 && x % 10 == 0) {
                this->draw(x, y, 0xFF333333);
            }
        }
    }
}

void ColorBuffer::drawRect(int x, int y, int width, int height, uint32_t color) {
    for (int rectY = y; rectY < y + height; rectY++) {
        for (int rectX = x; rectX < x + width; rectX++) {
            this->draw(rectX, rectY, color);
        }
    }
}

void ColorBuffer::drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    int dX = x1 - x0;
    int dY = y1 - y0;
    int stepSize = abs(dX) >= abs(dY) ? abs(dX) : abs(dY);

    float xInc = dX / (float) stepSize;
    float yInc = dY / (float) stepSize;

    float currX = x0;
    float currY = y0;

    for (int i = 0; i <= stepSize; i++) {
        this->draw(round(currX), round(currY), color);
        currX += xInc;
        currY += yInc;
    }
}

void ColorBuffer::drawFlatBottomTriangle(int x0, int y0, int x1, int y1, int mX, int mY) {
  float slope1 = (float) (x1 - x0) / (y1 - y0);
  float slope2 = (float) (mX - x0) / (mY - y0);

  float xStart = x0, xEnd = x0;

  for (int y = y0; y < mY; y++) {
    this->drawLine(xStart, y, xEnd, y, 0xFFFF0000);
    xStart += slope1;
    xEnd += slope2;
  }
}

void ColorBuffer::drawFlatTopTriangle(int x0, int y0, int mX, int mY, int x2, int y2) {
  float slope1 = (float) (x2 - x0) / (y2 - y0);
  float slope2 = (float) (x2 - mX) / (y2 - mY);

  float xStart = x2, xEnd = x2;

  for (int y = y2; y >= mY; y--) {
    this->drawLine(xStart, y, xEnd, y, 0xFFFF0000);
    xStart -= slope1;
    xEnd -= slope2;
  }
}

void    ColorBuffer::drawFilledTriangle(Triangle& triangle, uint32_t color) {
    if (triangle.points[0].y > triangle.points[1].y)
        std::swap(triangle.points[0], triangle.points[1]);
    if (triangle.points[1].y > triangle.points[2].y)
        std::swap(triangle.points[1], triangle.points[2]);
    if (triangle.points[0].y > triangle.points[1].y)
        std::swap(triangle.points[0], triangle.points[1]);

    if (triangle.points[1].y == triangle.points[2].y) {
      this->drawFlatBottomTriangle(
          triangle.points[0].x, triangle.points[0].y,
          triangle.points[1].x, triangle.points[1].y,
          triangle.points[2].x, triangle.points[2].y
      );
      return;
    }

    if (triangle.points[0].y == triangle.points[1].y) {
      this->drawFlatTopTriangle(
          triangle.points[0].x, triangle.points[0].y,
          triangle.points[1].x, triangle.points[1].y,
          triangle.points[2].x, triangle.points[2].y
      );
      return;
    }

    int mY = triangle.points[1].y;
    int mX = ((float)((triangle.points[2].x - triangle.points[0].x) * (triangle.points[1].y - triangle.points[0].y)) / (float)(triangle.points[2].y - triangle.points[0].y)) + triangle.points[0].x;

    this->drawFlatBottomTriangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, mX, mY);
    this->drawFlatTopTriangle(triangle.points[1].x, triangle.points[1].y, mX, mY, triangle.points[2].x, triangle.points[2].y);
}

void    ColorBuffer::drawWireframe(Triangle &triangle, uint32_t color) {
  this->drawLine(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, 0xFFF0F0F0);
  this->drawLine(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xFFF0F0F0);
  this->drawLine(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, 0xFFF0F0F0);
}

void ColorBuffer::render(SDL_Renderer *renderer) {
    SDL_UpdateTexture(
        this->_texture,
        NULL,
        this->_buffer.data(),
        this->_width * sizeof(uint32_t)
    );

    SDL_RenderCopy(
        renderer,
        this->_texture,
        NULL,
        NULL
    );
}

std::vector<uint32_t>& ColorBuffer::getBuffer() {
    return this->_buffer;
}

void ColorBuffer::setTexture(SDL_Texture* texture) {
    this->_texture = texture;
}

SDL_Texture* ColorBuffer::getTexture() {
    return this->_texture;
}