#pragma once
#include "Event.h"
#include "NamedSprite.h"

class CollisionEvent : public Event {
public:
    CollisionEvent(Timeline& timeline,
        NamedSprite& sprite,
        double lastValidX,
        double lastValidY,
        bool wasMovingDown,
        bool& isOnGround,
        bool isHorizontal);

    NamedSprite& getSprite() const;
    double getLastValidY() const;
    double getLastValidX() const;
    bool getWasMovingDown() const;
    bool& getIsOnGround();
    bool isHorizontalCollision() const;

private:
    NamedSprite& sprite;
    double lastValidY;
    double lastValidX;
    bool wasMovingDown;
    bool& isOnGround;
    bool isHorizontal;
};
