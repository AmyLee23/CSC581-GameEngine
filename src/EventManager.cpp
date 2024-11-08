#include "pch.h"
#include "EventManager.h"
#include <iostream>

void EventManager::registerHandler(const std::string& eventType,
    std::shared_ptr<EventHandler> handler) {
    std::lock_guard<std::mutex> lock(eventMutex);
    handlers[eventType].push_back(handler);
}

void EventManager::raiseEvent(std::shared_ptr<Event> event) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push(event);
}

void EventManager::update() {
    std::lock_guard<std::mutex> lock(eventMutex);

    while (!eventQueue.empty()) {
        auto event = eventQueue.top();
        eventQueue.pop();

        // Get handlers for this event type
        auto& eventHandlers = handlers[event->getType()];

        // Call all registered handlers
        for (auto& handler : eventHandlers) {
            handler->onEvent(event);
        }
    }
}

Timeline& EventManager::getTimeline() const {
    return timeline;
}

EventManager::~EventManager() {
    std::lock_guard<std::mutex> lock(eventMutex);
    while (!eventQueue.empty()) {
        eventQueue.pop();
    }
    handlers.clear();
}
