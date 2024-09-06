#include "FixedShape.h"

FixedShape::FixedShape(const std::string& name, float x, float y, int width, int height, Drawer& drawer)
    : Shape(name, x, y, width, height, drawer) {}

void FixedShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) {
    // fixed shapes do not move
}
