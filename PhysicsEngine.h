#pragma once

#include <unordered_map>
#include <SDL.h>

#include "Shape.h"
// #include "WindowManager.h"
class WindowManager;

class PhysicsEngine {
public:
    PhysicsEngine(WindowManager& windowManager);

    bool checkShapeCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes);
    bool checkBottomCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes);
    bool checkSideCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes);
    bool checkTopBottomCollision(const Shape* shape, const std::unordered_map<std::string, Shape*>& shapes);

    void applyGravity(Shape* shape);

    int getGravity();
    void setGravity(int newGravity);

private:
    int gravity = 1;

    WindowManager& windowManager;

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) const;
    bool checkWindowBounds(const Shape* shape) const;
};
