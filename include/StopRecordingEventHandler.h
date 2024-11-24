#pragma once
#include "EventHandler.h"
#include "StopRecordingEvent.h"
#include "ReplayManager.h"
#include <iostream>
class StopRecordingEventHandler : public EventHandler {
public:
    StopRecordingEventHandler(ReplayManager& manager) : replayManager(manager) {}

    void onEvent(std::shared_ptr<Event> event) override {
        auto stopEvent = std::static_pointer_cast<StopRecordingEvent>(event);
        replayManager.stopRecording();
        std::cout << "Stopped recording replay" << std::endl;
    }

private:
    ReplayManager& replayManager;
};