#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

class ColorBuffer {
    public:
    ColorBuffer();
    ColorBuffer(const int width, const int height);
    ColorBuffer(const ColorBuffer& other);

    ~ColorBuffer();

    ColorBuffer& operator=(const ColorBuffer& other);

    void clear(uint32_t color);
    void render(SDL_Renderer *renderer);

    std::vector<uint32_t>& getBuffer();

    SDL_Texture* getTexture();
    void setTexture(SDL_Texture* texture);

    private:
    int                     _width, _height;
    std::vector<uint32_t>   _buffer;
    SDL_Texture*            _texture;
};