#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

#include <SDL.h>

class PhysicsEngine;
class Game;

class ControllableShape : public Shape {
public:
    ControllableShape(const std::string& name, int x, int y, int width, int height);
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) override;
    void handleInput(const Uint8* keystate, Game& game);
    int getSpeed();
    void setSpeed(int speed);
    //void handleEvent(const SDL_Event& e);

private:
    int speed = 1;
};
