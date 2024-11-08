#pragma once
#include "Event.h"
#include "Timeline.h"
#include "NamedSprite.h"

class SpawnEvent : public Event {
private:
    NamedSprite& sprite;

public:
    SpawnEvent(Timeline& timeline, NamedSprite& sprite)
        : Event(timeline, "Spawn", 1),
        sprite(sprite) {}

    NamedSprite& getSprite() const { return sprite; }
};