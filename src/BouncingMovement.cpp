#include "pch.h"
#include "BouncingMovement.h"
#include "PhysicsEngine.h"
#include "GameWorld.h"
#include "NamedSprite.h"
#include <iostream>

BouncingMovement::BouncingMovement(double dx, double dy)
    : dx(dx), dy(dy) {}

void BouncingMovement::move(NamedSprite& sprite, GameState& gameState, double dt) {
    PositionComponent* position = sprite.getPosition();
    PhysicsEngine& physics = gameState.getPhysicsEngine();
    GameWorld& gameWorld = gameState.getGameWorld();

    double metersMovedX = dx * dt;
    double metersMovedY = dy * dt;
    double moveX = metersMovedX * physics.getPixelsPerMeter();
    double moveY = metersMovedY * physics.getPixelsPerMeter();
    //std::cout << "Move x: " << moveX << std::endl;
    double lastX = position->getX();
    double lastY = position->getY();

    // try x movement
    position->setX(lastX + moveX);
    // Check for collisions with GameWorld bounds
    if (position->getX() < 0 || position->getX() + position->getWidth() > gameWorld.getWorldWidth()) {
        // If collision occurs on the left or right side, reverse horizontal velocity (bounce)
        //std::cout << "x collision world: " << (lastX + moveX) << std::endl;
        dx = -dx;
        position->setX(lastX);
    } else if (physics.checkCollisions(sprite, gameState)) {
        //std::cout << "x collision shape: " << (lastX + moveX) << std::endl;
        dx = -dx;  // Bounce horizontally
        position->setX(lastX);
    }

    //try y movement
    position->setY(lastY + moveY);
    if (position->getY() < 0 || position->getY() + position->getHeight() > gameWorld.getWorldHeight()) {
        // If collision occurs on the top or bottom side, reverse vertical velocity (bounce)
        dy = -dy;
        position->setY(lastY);
    } else if (physics.checkCollisions(sprite, gameState)) {
        dy = -dy;  // Bounce vertically
        position->setY(lastY);
    }
}

double BouncingMovement::getDx() const {
    return dx;
}

double BouncingMovement::getDy() const {
    return dy;
}

void BouncingMovement::setDx(double dx) {
    this->dx = dx;
}

void BouncingMovement::setDy(double dy) {
    this->dy = dy;
}
