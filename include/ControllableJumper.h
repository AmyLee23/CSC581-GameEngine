#pragma once

#include "MovementComponent.h"
#include "InputComponent.h"
#include <mutex>

class ControllableJumper : public MovementComponent, public InputComponent {
private:
    double dx;            // Horizontal movement speed
    double dy;            // Vertical movement speed
    bool isJumping;      // Indicates if the sprite is currently jumping
    bool isOnGround;     // Indicates if the sprite is on the ground
    double jumpVelocity;  // The velocity when jumping
    double speed;
    std::mutex mtx;

    // Dash properties
    bool isDashing = false;
    double dashSpeedMultiplier = 3.0; // Adjust multiplier as needed
    const double dashDuration = 0.25; // Dash duration in seconds
    std::chrono::steady_clock::time_point dashStartTime;

public:
    ControllableJumper(double dx, double dy, double jumpVelocity);

    double getDx() const override;
    double getDy() const override;
    double getJumpVelocity() const;
    bool getIsJumping() const;
    bool getIsOnGround() const;

    void setDx(double dx) override;
    void setDy(double dy) override;
    void setJumpVelocity(double jumpVelocity);
    void setIsJumping(bool isJumping);
    void setIsOnGround(bool isOnGround);

    // Add a method to initiate a dash
    void startDash(int direction);

    void move(NamedSprite& sprite, GameState& gameState, double dt) override;
    void handleInput(const Uint8* keystate, NamedSprite& sprite, GameState& gameState, double dt) override;
};
