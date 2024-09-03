#include "Shape.h"
#include <SDL.h>

Shape::Shape(int x, int y, int w, int h, SDL_Color color)
    : x_(x), y_(y), w_(w), h_(h), color_(color), windowWidth_(800), windowHeight_(600) { }

void Shape::move(int dx, int dy) {
    x_ += dx;
    y_ += dy;

    // Ensure the shape stays within window boundaries
    if (x_ < 0) x_ = 0;
    if (x_ + w_ > windowWidth_) x_ = windowWidth_ - w_;
    if (y_ < 0) y_ = 0;
    if (y_ + h_ > windowHeight_) y_ = windowHeight_ - h_;
}

void Shape::draw(SDL_Renderer* renderer) const {
    SDL_Rect fillRect = { x_, y_, w_, h_ };
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &fillRect);
}

void Shape::setBounds(int windowWidth, int windowHeight) {
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;
}

// New methods
int Shape::getY() const {
    return y_;
}

void Shape::setY(int y) {
    y_ = y;
}
