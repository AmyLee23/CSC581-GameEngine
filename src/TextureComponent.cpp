#include "pch.h"

#include "TextureComponent.h"
#include <iostream>

TextureComponent::TextureComponent(SDL_Renderer* renderer, const std::string& filePath)
    : texture(nullptr), loaded(false) {
    // Load BMP file into an SDL_Surface
    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load BMP: " << filePath << " Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Create texture from surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface after creating the texture

    if (texture) {
        loaded = true;
    }
    else {
        std::cerr << "Failed to create texture from BMP: " << filePath << " Error: " << SDL_GetError() << std::endl;
    }
}

TextureComponent::~TextureComponent() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

SDL_Texture* TextureComponent::getTexture() const {
    return texture;
}

bool TextureComponent::isLoaded() const {
    return loaded;
}
