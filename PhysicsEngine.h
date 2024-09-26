#pragma once

#include <unordered_map>
#include <SDL.h>
#include "PositionComponent.h"
#include "GameState.h"

class NamedSprite;

class PhysicsEngine {
public:
    PhysicsEngine(double gravity = 15.0, double pixelsPerMeter = 64.0);

    // Check collisions
    bool checkCollisions(NamedSprite& sprite, GameState& gameState);
    //bool checkBottomCollision(NamedSprite& sprite, GameState& gameState);
    //bool checkTopCollision(NamedSprite& sprite, GameState& gameState);
    //bool checkLeftCollision(NamedSprite& sprite, GameState& gameState);
    //bool checkRightCollision(NamedSprite& sprite, GameState& gameState);

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) const;
    bool checkWorldBounds(NamedSprite& sprite, GameState& gameState);

    // Gravity related functions
    void applyGravity(NamedSprite& sprite, double dt);
    double getGravity() const;
    void setGravity(double newGravity);

    double getPixelsPerMeter() const;
    void setPixelsPerMeter(double newPixelsPerMeter);

    SDL_Rect makeRect(PositionComponent& position);

private:
    double gravity;
    double pixelsPerMeter;
};

