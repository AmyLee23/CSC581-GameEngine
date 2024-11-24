#pragma once
#include "Event.h"
#include "NamedSprite.h"

class PlayReplayEvent : public Event {
private:
    NamedSprite& sprite;

public:
    PlayReplayEvent(Timeline& timeline, NamedSprite& sprite)
        : Event(timeline, "PlayReplay", 4),  // High priority like input
        sprite(sprite) {}

    NamedSprite& getSprite() const { return sprite; }
};