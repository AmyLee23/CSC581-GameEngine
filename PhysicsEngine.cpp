#include "PhysicsEngine.h"
#include "WindowManager.h"

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

bool PhysicsEngine::checkCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    return SDL_HasIntersection(&a, &b);
}

bool PhysicsEngine::checkWindowBounds(const Shape* shape) const {
    return shape->getX() < 0 || shape->getY() < 0 ||
        shape->getX() + shape->getWidth() > windowManager.getWindowWidth() ||
        shape->getY() + shape->getHeight() > windowManager.getWindowHeight();
}
