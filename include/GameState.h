#pragma once

#include <SDL.h>
#include "EventManager.h"
#include "JClient.h"
#include "ReplayManager.h"

class Timeline;
class GameWorld;
class PhysicsEngine;
class WindowManager;
class GameTypes;

class GameState {
private:
    Timeline& globalTimeline;
    GameWorld& gameWorld;
    PhysicsEngine& physicsEngine;
    WindowManager& windowManager;
    GameTypes& gameTypes;
    EventManager eventManager;
    std::queue<std::shared_ptr<Event>> pendingEvents;
    ReplayManager replayManager;
    JClient* client;

public:
    GameState(Timeline& timeline, GameWorld& world, PhysicsEngine& physics, WindowManager& window, GameTypes& types);
    GameState(Timeline& timeline, GameWorld& world, PhysicsEngine& physics, WindowManager& window, GameTypes& types, JClient& client);
    ~GameState();

    void step(const Uint8* keystate);
    // step split up (for multi threading)
    void stepInput(const Uint8* keystate);
    void stepMovement();
    void applyGameState(const std::unordered_map<std::string, std::tuple<size_t, double, double>>& state,
        GameState& gameState);
    void stepReplay();


    Timeline& getTimeline();
    GameWorld& getGameWorld();
    PhysicsEngine& getPhysicsEngine();
    WindowManager& getWindowManager();
    GameTypes& getGameTypes();
    EventManager& getEventManager() { return eventManager; }
    ReplayManager& getReplayManager() { return replayManager; }

    void queueEvent(std::shared_ptr<Event> event);
    void processPendingEvents();
};