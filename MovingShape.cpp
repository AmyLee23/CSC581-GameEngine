#include "MovingShape.h"

MovingShape::MovingShape(const std::string& name, int x, int y, int width, int height, int dx, int dy)
    : Shape(name, x, y, width, height), dx(dx), dy(dy) {}

void MovingShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) {
    lastX = x;
    lastY = y;

    x += dx;
    y += dy;

    if (physics.checkShapeCollision(this, shapes)) {
        moveBack();
        dx = -dx;
        dy = -dy;
    }
}
