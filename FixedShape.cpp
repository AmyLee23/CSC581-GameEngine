#include "FixedShape.h"

FixedShape::FixedShape(const std::string& name, int x, int y, int width, int height)
    : Shape(name, x, y, width, height) {}

void FixedShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) {
    // fixed shapes do not move
}
