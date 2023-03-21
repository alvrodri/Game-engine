#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>

class ColorBuffer {
    public:
    ColorBuffer();
    ColorBuffer(const int width, const int height);
    ColorBuffer(const ColorBuffer& other);

    ~ColorBuffer();

    ColorBuffer& operator=(const ColorBuffer& other);

    void clear(uint32_t color);
    void render(SDL_Renderer *renderer);
    void draw(int x, int y, uint32_t color);

    uint32_t getColor(int x, int y);

    void    drawGrid();
    void    drawRect(int x, int y, int width, int height, uint32_t color);
    void    drawLine(int x0, int y0, int x1, int y1, uint32_t color);

    std::vector<uint32_t>& getBuffer();

    SDL_Texture* getTexture();
    void setTexture(SDL_Texture* texture);

    private:
    int                     _width, _height;
    std::vector<uint32_t>   _buffer;
    SDL_Texture*            _texture;
};