#pragma once

#include <SDL.h>

class Shape {
public:
    Shape(int x, int y, int w, int h, SDL_Color color);
    void setBounds(int windowWidth, int windowHeight);
    void move(int dx, int dy);
    void draw(SDL_Renderer* renderer) const;

private:
    int x_, y_;
    int w_, h_;
    SDL_Color color_;
    int windowWidth_, windowHeight_;
};
