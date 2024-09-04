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

private:
    WindowManager& windowManager;

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) const;
    bool checkWindowBounds(const Shape* shape) const;
};
