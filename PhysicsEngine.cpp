#include "PhysicsEngine.h"
#include "WindowManager.h"
#include "ControllableShape.h"
#include <tuple>
#include <iostream>

PhysicsEngine::PhysicsEngine(WindowManager& windowManager)
    : windowManager(windowManager) {}

SDL_Rect PhysicsEngine::makeRect(const Shape* shape) {
    SDL_Rect rect = { static_cast<int>(shape->getX()), static_cast<int>(shape->getY()), shape->getWidth(), shape->getHeight() };
    return rect;
}

bool PhysicsEngine::checkShapeCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes) {
    SDL_Rect rect = makeRect(shape);

    // check collision with other shapes
    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = makeRect(otherShape);
            if (checkCollision(rect, otherRect)) {
                return true;
            }
        }
    }

    // check collision with window bounds
    return checkWindowBounds(shape);
}

bool PhysicsEngine::checkBottomCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes) {
    SDL_Rect rect = makeRect(shape);
    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = makeRect(otherShape);
            if (checkCollision(rect, otherRect)) {
                //if (rect.y + rect.h <= otherRect.y) {
                if ((rect.y + rect.h >= otherRect.y && rect.y + rect.h <= otherRect.y + 2.0f) &&  // Bottom is close to top
                    (rect.x + rect.w > otherRect.x && rect.x < otherRect.x + otherRect.w)) {  // Horizontal overlap
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
    SDL_Rect rect = makeRect(shape);
    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = makeRect(otherShape);
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
    SDL_Rect rect = makeRect(shape);
    for (const auto& pair : shapes) {
        const Shape* otherShape = pair.second;
        if (shape != otherShape) {
            SDL_Rect otherRect = makeRect(otherShape);
            if (checkCollision(rect, otherRect)) {
                // if the top or bottom of the shape touches another shape
                if ((rect.y <= otherRect.y + otherRect.h && rect.y >= otherRect.y) ||  // top collision
                    (rect.y + rect.h >= otherRect.y && rect.y <= otherRect.y)) {       // bottom collision
                    return true;
                }
            }
        }
    }
    if (rect.y <= 0.0f || rect.y + rect.h >= windowManager.getWindowHeight()) {
        return true;
    }
    return false;
}


bool PhysicsEngine::checkCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    return SDL_HasIntersection(&a, &b);
}

bool PhysicsEngine::checkWindowBounds(const Shape* shape) const {
    return shape->getX() < 0.0f || shape->getY() < 0.0f ||
        shape->getX() + shape->getWidth() > windowManager.getWindowWidth() ||
        shape->getY() + shape->getHeight() > windowManager.getWindowHeight();
}

float PhysicsEngine::getGravity() { return gravity; }
void PhysicsEngine::setGravity(float newGravity) { gravity = newGravity; }

void PhysicsEngine::applyGravity(Shape* shape, float dt) {
    if (auto controllable = dynamic_cast<ControllableShape*>(shape)) {
        std::tuple<float, float> speed = controllable->getSpeed();
        float dx = std::get<0>(speed);
        float dy = std::get<1>(speed);
        float metersMovedY = dy * dt;
        float moveY = metersMovedY * getPixelsPerMeter();

        controllable->setY(controllable->getY() - moveY);
        dy -= gravity * dt;
        //std::cout << "dy under gravity: " << dy << " m/s" << std::endl;
        controllable->setSpeed(dx, dy);
    }
}

float PhysicsEngine::getPixelsPerMeter() { return pixelsPerMeter; }
void PhysicsEngine::setPixelsPerMeter(float newPixelsPerMeter) { pixelsPerMeter = newPixelsPerMeter; }
