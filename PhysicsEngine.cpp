#include "PhysicsEngine.h"
#include "WindowManager.h"
#include "ControllableShape.h"
#include <tuple>

PhysicsEngine::PhysicsEngine(WindowManager& windowManager)
    : windowManager(windowManager) {}

bool PhysicsEngine::checkShapeCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes) {
    SDL_Rect rect = { shape->getX(), shape->getY(), shape->getWidth(), shape->getHeight() };

    // check collision with other shapes
    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = { otherShape->getX(), otherShape->getY(), otherShape->getWidth(), otherShape->getHeight() };
            if (checkCollision(rect, otherRect)) {
                return true;
            }
        }
    }

    // check collision with window bounds
    return checkWindowBounds(shape);
}

bool PhysicsEngine::checkBottomCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes) {
    SDL_Rect rect = { shape->getX(), shape->getY(), shape->getWidth(), shape->getHeight() };

    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = { otherShape->getX(), otherShape->getY(), otherShape->getWidth(), otherShape->getHeight() };
            if (checkCollision(rect, otherRect)) {
                if (rect.y + rect.h <= otherRect.y + 2) { // small tolerance?
                    return true;
                }
            }
        }
    }
    if (rect.y + rect.h >= windowManager.getWindowHeight()) {
        return true;
    }
    return false;
}

bool PhysicsEngine::checkSideCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes) {
    SDL_Rect rect = { shape->getX(), shape->getY(), shape->getWidth(), shape->getHeight() };

    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = { otherShape->getX(), otherShape->getY(), otherShape->getWidth(), otherShape->getHeight() };
            if (checkCollision(rect, otherRect)) {
                // if the shape is colliding on the left or right side
                if ((rect.x <= otherRect.x + otherRect.w && rect.x >= otherRect.x) ||  // left side
                    (rect.x + rect.w >= otherRect.x && rect.x <= otherRect.x)) {       // right side
                    return true;
                }
            }
        }
    }
    if (rect.x <= 0 || rect.x + rect.w >= windowManager.getWindowWidth()) {
        return true;
    }
    return false;
}

bool PhysicsEngine::checkTopBottomCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes) {
    SDL_Rect rect = { shape->getX(), shape->getY(), shape->getWidth(), shape->getHeight() };

    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = { otherShape->getX(), otherShape->getY(), otherShape->getWidth(), otherShape->getHeight() };
            if (checkCollision(rect, otherRect)) {
                // if the top or bottom of the shape touches another shape
                if ((rect.y <= otherRect.y + otherRect.h && rect.y >= otherRect.y) ||  // top collision
                    (rect.y + rect.h >= otherRect.y && rect.y <= otherRect.y)) {       // tottom collision
                    return true;
                }
            }
        }
    }
    if (rect.y <= 0 || rect.y + rect.h >= windowManager.getWindowHeight()) {
        return true;
    }
    return false;
}


bool PhysicsEngine::checkCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    return SDL_HasIntersection(&a, &b);
}

bool PhysicsEngine::checkWindowBounds(const Shape* shape) const {
    return shape->getX() < 0 || shape->getY() < 0 ||
        shape->getX() + shape->getWidth() > windowManager.getWindowWidth() ||
        shape->getY() + shape->getHeight() > windowManager.getWindowHeight();
}

int PhysicsEngine::getGravity() { return gravity; }
void PhysicsEngine::setGravity(int newGravity) { gravity = newGravity; }

void PhysicsEngine::applyGravity(Shape* shape) {
    if (auto controllable = dynamic_cast<ControllableShape*>(shape)) {
        std::tuple<int, int> speed = controllable->getSpeed();
        int dx = std::get<0>(speed);
        int dy = std::get<1>(speed);

        controllable->setY(controllable->getY() - dy);
        dy -= gravity;
        controllable->setSpeed(dx, dy);
    }
}
