#pragma once

#include <SDL.h>

class Shape {
public:
    Shape(int x, int y, int w, int h, SDL_Color color);

    void move(int dx, int dy);
    void draw(SDL_Renderer* renderer) const;
    void setBounds(int windowWidth, int windowHeight);

    // Add these methods
    int getY() const;      // Getter for y-coordinate
    void setY(int y);      // Setter for y-coordinate

private:
    int x_;
    int y_;
    int w_;
    int h_;
    SDL_Color color_;
    int windowWidth_;
    int windowHeight_;
};

