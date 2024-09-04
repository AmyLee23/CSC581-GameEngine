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

    void applyGravity(Shape* shape, float dt);

    float getGravity();
    void setGravity(float newGravity);

    float getPixelsPerMeter();
    void setPixelsPerMeter(float newPixelsPerMeter);

    SDL_Rect makeRect(const Shape* shape);

private:
    float gravity = 15.0f;
    float pixelsPerMeter = 64.0f;

    WindowManager& windowManager;

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) const;
    bool checkWindowBounds(const Shape* shape) const;
};
