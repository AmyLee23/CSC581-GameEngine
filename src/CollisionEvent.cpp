#include "pch.h"
#include "CollisionEvent.h"
#include "Timeline.h"

CollisionEvent::CollisionEvent(Timeline& timeline,
    NamedSprite& sprite,
    double lastValidX,
    double lastValidY,
    bool wasMovingDown,
    bool& isOnGround,
    bool isHorizontal)
    : Event(timeline, "Collision", 3),  // Set type to "Collision" and priority to 3
    sprite(sprite),
    lastValidX(lastValidX),
    lastValidY(lastValidY),
    wasMovingDown(wasMovingDown),
    isOnGround(isOnGround),
    isHorizontal(isHorizontal) {}

NamedSprite& CollisionEvent::getSprite() const {
    return sprite;
}

double CollisionEvent::getLastValidY() const {
    return lastValidY;
}

double CollisionEvent::getLastValidX() const {
    return lastValidX;
}

bool CollisionEvent::getWasMovingDown() const {
    return wasMovingDown;
}

bool& CollisionEvent::getIsOnGround() {
    return isOnGround;
}

bool CollisionEvent::isHorizontalCollision() const {
    return isHorizontal;
}