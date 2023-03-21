#include "ColorBuffer.hpp"

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