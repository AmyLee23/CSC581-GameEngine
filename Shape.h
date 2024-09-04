#ifndef SHAPE_H
#define SHAPE_H

#include <SDL.h>

class Shape {
public:
    Shape(int x, int y, int w, int h, bool proportionalMode, SDL_Color color);

    void move(int dx, int dy);
    void draw(SDL_Renderer* renderer) const;
    void setBounds(int windowWidth, int windowHeight);
    void proportionality(bool &proportionalMode);

    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);

    int getWidth() const;
    int getHeight() const;

private:
    int x_, y_;
    int w_, h_;
    SDL_Color color_;
    int windowWidth_;
    int windowHeight_;
    bool proportionalMode_;
};

#endif // SHAPE_H
