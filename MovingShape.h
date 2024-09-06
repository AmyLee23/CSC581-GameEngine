#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

class MovingShape : public Shape {
public:
    MovingShape(const std::string& name, float x, float y, int width, int height, Drawer& drawer, float dx, float dy, SDL_Color color = { 255, 0, 0, 255 });
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) override;

protected:
    float dx, dy;
};
