#include "FixedShape.h"

FixedShape::FixedShape(const std::string& name, float x, float y, int width, int height)
    : Shape(name, x, y, width, height) {}

void FixedShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) {
    // fixed shapes do not move
}
