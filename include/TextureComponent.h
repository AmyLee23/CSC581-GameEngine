#pragma once
#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include <SDL.h>
#include <string>

class TextureComponent {
public:
    TextureComponent(SDL_Renderer* renderer, const std::string& filePath);
    ~TextureComponent();

    SDL_Texture* getTexture() const;
    bool isLoaded() const;

private:
    SDL_Texture* texture;
    bool loaded;
};

#endif // TEXTURE_COMPONENT_H
