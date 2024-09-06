#include "MovingShape.h"

MovingShape::MovingShape(const std::string& name, float x, float y, int width, int height, Drawer& drawer, float dx, float dy, SDL_Color color)
    : Shape(name, x, y, width, height, drawer, color), dx(dx), dy(dy) {}

void MovingShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) {
    lastX = x;
    lastY = y;

    float metersMovedX = dx * dt;  // Distance in meters (speed * time)
    float metersMovedY = dy * dt;
    float moveX = metersMovedX * physics.getPixelsPerMeter();
    float moveY = metersMovedY * physics.getPixelsPerMeter();

    x += moveX;
    y += moveY;

    if (physics.checkShapeCollision(this, shapes)) {
        moveBack();
        dx = -dx;
        dy = -dy;
    }
}
