#pragma once

#include <SDL.h>

class Shape;
class Game;

class Drawer {
public:
    Drawer();
    ~Drawer();
    void drawShape(Game& game, Shape* shape, bool scaling);
    void prepareRelativePosition(Shape* shape, Game& game);
    void prepareScaling(Shape* shape, Game& game);
};