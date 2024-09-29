#include "pch.h"
#include "PhysicsEngine.h"
#include "NamedSprite.h"
#include "GameWorld.h"
#include "NamedSprite.h"
#include <iostream>

PhysicsEngine::PhysicsEngine(double gravity, double pixelsPerMeter)
    : gravity(gravity), pixelsPerMeter(pixelsPerMeter) {}

// Create an SDL_Rect from PositionComponent
SDL_Rect PhysicsEngine::makeRect(PositionComponent& position) {
    SDL_Rect rect = {
        static_cast<int>(position.getX()),
        static_cast<int>(position.getY()),
        position.getWidth(),
        position.getHeight()
    };
    return rect;
}

bool PhysicsEngine::checkCollisions(NamedSprite& sprite, GameState& gameState) {
    PositionComponent* position = sprite.getPosition();
    SDL_Rect rect = makeRect(*position);
    for (auto& pair : gameState.getGameWorld().getObjects()) {
        NamedSprite* otherSprite = pair.second;
        if (sprite.getName() != otherSprite->getName()) {
            SDL_Rect otherRect = makeRect(*otherSprite->getPosition());
            if (checkCollision(rect, otherRect)) {
                return true;
            }
        }
    }
    return false;
}

// Check if two SDL_Rects are colliding
bool PhysicsEngine::checkCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    return SDL_HasIntersection(&a, &b);
}

// Check if the position is out of world bounds
bool PhysicsEngine::checkWorldBounds(NamedSprite& sprite, GameState& gameState) {
    PositionComponent& position = *sprite.getPosition();
    return position.getX() < 0 || position.getY() < 0 ||
        position.getX() + position.getWidth() > gameState.getGameWorld().getWorldWidth() ||
        position.getY() + position.getHeight() > gameState.getGameWorld().getWorldHeight();
}

// Apply gravity to the position
void PhysicsEngine::applyGravity(NamedSprite& sprite, double dt) {
    PositionComponent& position = *sprite.getPosition();
    MovementComponent& movement = *sprite.getMovement();
    double newY = position.getY() - movement.getDy() * pixelsPerMeter * dt;
    double newDy = movement.getDy() - gravity * dt;
    position.setY(newY);
    movement.setDy(newDy);
}

// Getters and Setters for gravity and pixels-per-meter
double PhysicsEngine::getGravity() const { return gravity; }
void PhysicsEngine::setGravity(double newGravity) { gravity = newGravity; }

double PhysicsEngine::getPixelsPerMeter() const { return pixelsPerMeter; }
void PhysicsEngine::setPixelsPerMeter(double newPixelsPerMeter) { pixelsPerMeter = newPixelsPerMeter; }
