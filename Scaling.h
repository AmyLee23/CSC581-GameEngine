#pragma once
#ifndef SCALING_H
#define SCALING_H

#include <SDL.h>

enum ScalingMode { CONSTANT_SIZE, PROPORTIONAL };

class Scaling {
public:
    Scaling(ScalingMode mode = CONSTANT_SIZE);

    void setMode(ScalingMode mode);
    void scaleRect(SDL_Rect& rect, int windowWidth, int windowHeight);

private:
    ScalingMode mode_;
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
};

#endif // SCALING_H
