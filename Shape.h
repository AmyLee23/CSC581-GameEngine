#ifndef SHAPE_H
#define SHAPE_H

#include <SDL.h>

class Shape {
public:
    Shape(int x, int y, int w, int h, SDL_Color color);
    void move(float dx, float dy);
    void draw(SDL_Renderer* renderer) const;
    void setBounds(int windowWidth, int windowHeight);
    void setProportionalScaling(bool enable);
    void applyScaling(int windowWidth, int windowHeight);
    void resetSize(int originalW, int originalH);

    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);

    int getWidth() const;
    int getHeight() const;

private:
    int x_, y_;
    int w_, h_;
    int originalW_, originalH_;
    SDL_Color color_;
    int windowWidth_, windowHeight_;
    bool proportionalScaling_;
};

#endif // SHAPE_H
