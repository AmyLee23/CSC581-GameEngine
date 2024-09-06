#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

#include <SDL.h>
#include <tuple>

class PhysicsEngine;
class Game;

class ControllableShape : public Shape {
public:
    ControllableShape(const std::string& name, float x, float y, int width, int height, Drawer& drawer, float dx = 5.0f, float dy = 0.0f, bool flying = false);
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) override;
    void handleInput(const Uint8* keystate, Game* game, float dt);

    std::tuple<float, float> getSpeed() const;
    void setSpeed(float dx, float dy);

    float getJumpVelocity();
    void setJumpVelocity(float newJumpVelocity);

    bool getIsJumping();
    bool getIsOnGround();
    bool getFlying();

    void setIsJumping(bool jump);
    void setIsOnGround(bool ground);
    void setFlying(bool flying);

private:
    float dx, dy;
    bool flying;

    bool isJumping = false;
    bool isOnGround = false;
    float jumpVelocity = 8.0f;
};
