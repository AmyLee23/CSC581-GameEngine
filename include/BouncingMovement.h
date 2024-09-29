#pragma once

#include "MovementComponent.h"
// #include "PositionComponent.h"
#include "GameState.h"
#include <cmath>

class BouncingMovement : public MovementComponent {
private:
    double dx;  // Movement in x direction (velocity)
    double dy;  // Movement in y direction (velocity)

public:
    BouncingMovement(double dx, double dy);

    /*void move(PositionComponent* position, GameState& gameState, double dt) override;*/
    void move(NamedSprite& sprite, GameState& gameState, double dt) override;

    double getDx() const override;
    double getDy() const override;

    void setDx(double dx) override;
    void setDy(double dy) override;
};