#include "Scaling.h"

Scaling::Scaling(ScalingMode mode) : mode_(mode) {}

void Scaling::setMode(ScalingMode mode) {
    mode_ = mode;
}

void Scaling::scaleRect(SDL_Rect& rect, int windowWidth, int windowHeight) {
    if (mode_ == PROPORTIONAL) {
        rect.w = rect.w * windowWidth / SCREEN_WIDTH;
        rect.h = rect.h * windowHeight / SCREEN_HEIGHT;
    }
    // For CONSTANT_SIZE, do nothing
}
