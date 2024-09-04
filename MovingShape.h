#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

class MovingShape : public Shape {
public:
    MovingShape(const std::string& name, float x, float y, int width, int height, float dx, float dy);
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) override;

protected:
    float dx, dy;
};
