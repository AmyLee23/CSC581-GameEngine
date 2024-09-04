#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

class MovingShape : public Shape {
public:
    MovingShape(const std::string& name, int x, int y, int width, int height, int dx, int dy);
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) override;

protected:
    int dx, dy;
};
