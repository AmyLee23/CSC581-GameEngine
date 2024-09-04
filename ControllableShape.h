#pragma once

#include "Shape.h"
#include "PhysicsEngine.h"

#include <SDL.h>
#include <tuple>

class PhysicsEngine;
class Game;

class ControllableShape : public Shape {
public:
    ControllableShape(const std::string& name, int x, int y, int width, int height, int dx = 1, int dy = 1, bool flying = false);
    void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) override;
    void handleInput(const Uint8* keystate, Game& game);

    std::tuple<int, int> getSpeed() const;
    void setSpeed(int dx, int dy);

    int getJumpVelocity();
    void setJumpVelocity(int newJumpVelocity);

    bool getIsJumping();
    bool getIsOnGround();
    bool getFlying();

    void setIsJumping(bool jump);
    void setIsOnGround(bool ground);
    void setFlying(bool flying);

private:
    int dx, dy;
    bool flying;

    bool isJumping = false;
    bool isOnGround = false;
    int jumpVelocity = 1;
};
