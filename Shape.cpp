#include "Shape.h"
#include <SDL.h>
#include "init.h"

Shape::Shape(int x, int y, int w, int h, SDL_Color color)
    : x_(x), y_(y), w_(w), h_(h), originalW_(w), originalH_(h), color_(color),
    windowWidth_(INITIAL_WIDTH), windowHeight_(INITIAL_HEIGHT), proportionalScaling_(false) { }

void Shape::move(float dx, float dy) {
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
    if (proportionalScaling_) {
        applyScaling(windowWidth, windowHeight);
    }
}

void Shape::setProportionalScaling(bool enable) {
    proportionalScaling_ = enable;
    if (!proportionalScaling_) {
        resetSize(originalW_, originalH_);  // calls reset size to reset the size if cancel proportional
    }
}

void Shape::applyScaling(int windowWidth, int windowHeight) {
    // Example scaling logic: Resize based on window dimensions
    // Adjust these factors as needed
    float scaleX = static_cast<float>(windowWidth) / INITIAL_WIDTH; // scale based on window width
    float scaleY = static_cast<float>(windowHeight) / INITIAL_HEIGHT; // scale based on window height
    float scale = (scaleX + scaleY) / 2.0f; // Average scaling factor for proportional scaling

    // Update width and height based on scaling factor
    w_ = static_cast<int>(originalW_ * scale);
    h_ = static_cast<int>(originalH_ * scale);
}

void Shape::resetSize(int originalW, int originalH) {
    w_ = originalW;
    h_ = originalH;

}

int Shape::getX() const {
    return x_;
}

void Shape::setX(int x) {
    x_ = x;
}

int Shape::getY() const {
    return y_;
}

void Shape::setY(int y) {
    y_ = y;
}

int Shape::getWidth() const {
    return w_;
}

int Shape::getHeight() const {
    return h_;
}
