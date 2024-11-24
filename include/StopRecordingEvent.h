#pragma once
#include "Event.h"
#include "NamedSprite.h"
class StopRecordingEvent : public Event {
public:
    StopRecordingEvent(Timeline& timeline)
        : Event(timeline, "StopRecording", 4) {}
};