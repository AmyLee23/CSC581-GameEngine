#pragma once
#include "Event.h"
#include "Timeline.h"
#include "NamedSprite.h"

class DeathEvent : public Event {
private:
    NamedSprite& sprite;

public:
    DeathEvent(Timeline& timeline, NamedSprite& sprite)
        : Event(timeline, "Death", 2),  // Priority 2
        sprite(sprite) {}

    NamedSprite& getSprite() const { return sprite; }
};