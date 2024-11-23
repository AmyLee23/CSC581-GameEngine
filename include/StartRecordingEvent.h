#pragma once
#include "Event.h"
#include "NamedSprite.h"
class StartRecordingEvent : public Event {
public:
    StartRecordingEvent(Timeline& timeline, NamedSprite& sprite)
        : Event(timeline, "StartRecording", 4),  // High priority
        sprite(sprite) {}

    NamedSprite& getSprite() const { return sprite; }
    
private:
	NamedSprite& sprite;
};