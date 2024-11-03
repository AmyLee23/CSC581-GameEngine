// EventManager.h
#pragma once

#include "Event.h"
#include "Timeline.h"
#include "GameState.h"
#include <queue>
#include <memory>
#include <string>

class EventManager {
public:
    EventManager(GameState& gameState);

    void registerEvent(const std::shared_ptr<Event>& event);
    void raiseEvent(const std::shared_ptr<Event>& event);
    void handleEvents();

    // New network methods
    std::string serializeEvent(const Event& event);
    std::shared_ptr<Event> deserializeEvent(const std::string& data);

private:
    GameState& gameState;
    std::priority_queue<std::shared_ptr<Event>> eventQueue;
};