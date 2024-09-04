#include "Shape.h"
#include <SDL.h>
#include "init.h"

Shape::Shape(int x, int y, int w, int h, bool proportionalMode, SDL_Color color)
    : x_(x), y_(y), w_(w), h_(h), color_(color), proportionalMode_(proportionalMode) { }

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
    float w = w_, h = h_;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (proportionalMode_) {
        w = abs(float(w_ * windowWidth_ )/ SCREEN_WIDTH);
        h = abs(float(h_ * windowHeight_ )/ SCREEN_HEIGHT);
    }
    SDL_Rect fillRect = { x_, y_, w, h };
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &fillRect);
}

void Shape::setBounds(int windowWidth, int windowHeight) {
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;
}

void Shape::proportionality(bool &proportionalMode) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        proportionalMode_ = !proportionalMode_;
    }
    proportionalMode = proportionalMode_;
}

// New methods
int Shape::getY() const {
    return y_;
}

void Shape::setY(int y) {
    y_ = y;
}
