#pragma once
#include "EventHandler.h"
#include "StartRecordingEvent.h"
#include "ReplayManager.h"
#include <iostream>

class StartRecordingEventHandler : public EventHandler {
public:
    StartRecordingEventHandler(ReplayManager& manager) : replayManager(manager) {}

    void onEvent(std::shared_ptr<Event> event) override {
        auto startEvent = std::static_pointer_cast<StartRecordingEvent>(event);
        replayManager.startRecording(startEvent->getSprite().getTimeline().getTime());
        std::cout << "Started recording replay" << std::endl;
    }

private:
    ReplayManager& replayManager;
};