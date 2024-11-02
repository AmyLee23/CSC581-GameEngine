#pragma once
#include "Timeline.h"
#include <mutex>
#include "EventHandler.h"
#include <unordered_map>
#include <queue>

class EventManager {
public:
    EventManager(Timeline& t) : timeline(t), eventQueue(), handlers() {}

    // Register event handlers
    void registerHandler(const std::string& eventType, std::shared_ptr<EventHandler> handler);
    // Raise (queue) an event
    void raiseEvent(std::shared_ptr<Event> event);
    void update();
    Timeline& getTimeline() const;
    ~EventManager();

private:
    Timeline& timeline;
    std::mutex eventMutex;
    std::unordered_map<std::string, std::vector<std::shared_ptr<EventHandler>>> handlers;
    // Custom comparator for priority queue
    struct EventComparator {
        bool operator()(const std::shared_ptr<Event>& e1,
            const std::shared_ptr<Event>& e2) {
            if (e1->getPriority() == e2->getPriority()) {
                return e1->getTimestamp() > e2->getTimestamp();
            }
            return e1->getPriority() < e2->getPriority();
        }
    };

    // Priority queue for events
    std::priority_queue<std::shared_ptr<Event>,
        std::vector<std::shared_ptr<Event>>,
        EventComparator> eventQueue;
};