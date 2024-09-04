#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

class FixedShape : public Shape {
public:
    FixedShape(const std::string& name, int x, int y, int width, int height);
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) override;
};
