#include "pch.h"
#include "ControllableJumper.h"
#include "NamedSprite.h"
#include "PhysicsEngine.h"
#include "GameWorld.h"
#include <iostream>

ControllableJumper::ControllableJumper(double dx, double dy, double jumpVelocity)
    : dx(dx), dy(dy), isJumping(false), isOnGround(false), jumpVelocity(jumpVelocity) {}

double ControllableJumper::getDx() const {
    return dx;
}

double ControllableJumper::getDy() const {
    return dy;
}

double ControllableJumper::getJumpVelocity() const {
    return jumpVelocity;
}

bool ControllableJumper::getIsJumping() const {
    return isJumping;
}

bool ControllableJumper::getIsOnGround() const {
    return isOnGround;
}

void ControllableJumper::setDx(double dx) {
    this->dx = dx;
}

void ControllableJumper::setDy(double dy) {
    this->dy = dy;
}

void ControllableJumper::setJumpVelocity(double jumpVelocity) {
    this->jumpVelocity = jumpVelocity;
}

void ControllableJumper::setIsJumping(bool isJumping) {
    this->isJumping = isJumping;
}

void ControllableJumper::setIsOnGround(bool isOnGround) {
    this->isOnGround = isOnGround;
}

void ControllableJumper::move(NamedSprite& sprite, GameState& gameState, double dt) {
    PositionComponent& position = *sprite.getPosition();
    PhysicsEngine& physics = gameState.getPhysicsEngine();

    double lastY = position.getY();

    physics.applyGravity(sprite, dt);
    //if (physics.checkSideCollision(this, shapes)) {
    //    moveBackX();
    //}
    if (physics.checkCollisions(sprite, gameState)) {
        dy = 0;
        if (lastY < position.getY()) {
            isOnGround = true;
        }
        position.setY(lastY);
    }
    //std::cout << "postion y: " << position.getY() << ", dy: " << dy << std::endl;
}

void ControllableJumper::handleInput(const Uint8* keystate, NamedSprite& sprite, GameState& gameState, double dt) {
    PositionComponent& position = *sprite.getPosition();
    GameWorld& gameWorld = gameState.getGameWorld();

    double lastX = position.getX();
    double lastY = position.getY();
    PhysicsEngine& physics = gameState.getPhysicsEngine();

    double metersMovedX = dx * dt;  // Distance in meters (speed * time)
    double metersMovedY = dy * dt;
    double moveX = metersMovedX * physics.getPixelsPerMeter();
    double moveY = metersMovedY * physics.getPixelsPerMeter();

    // Handle jumping
    if (keystate[SDL_SCANCODE_SPACE] && isOnGround) {
        dy = jumpVelocity;  // start jumping by setting the vertical velocity
        isJumping = true;
        isOnGround = false;
    }
    
    if (keystate[SDL_SCANCODE_LEFT]) {
        position.setX(position.getX() - moveX);
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        position.setX(position.getX() + moveX);
    }

    if (position.getX() < 0 || position.getX() + position.getWidth() > gameWorld.getWorldWidth() || physics.checkCollisions(sprite, gameState)) {
        position.setX(lastX);
    }
}
