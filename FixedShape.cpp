#include "FixedShape.h"

FixedShape::FixedShape(const std::string& name, float x, float y, int width, int height, Drawer& drawer, SDL_Color color)
    : Shape(name, x, y, width, height, drawer, color) {}

void FixedShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) {
    // fixed shapes do not move
}
