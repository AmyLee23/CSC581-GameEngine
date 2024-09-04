#include "Physics.h"
#include "SDL.h"

Physics::Physics(float gravity) : gravity_(gravity) {}

void Physics::applyGravity(float& velocityY) {
    velocityY += gravity_;
}
