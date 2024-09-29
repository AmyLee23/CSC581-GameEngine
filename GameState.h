#pragma once

#include <SDL.h>

class Timeline;
class GameWorld;
class PhysicsEngine;
class WindowManager;

class GameState {
private:
    Timeline& globalTimeline;
    GameWorld& gameWorld;
    PhysicsEngine& physicsEngine;
    WindowManager& windowManager;

public:
    GameState(Timeline& timeline, GameWorld& world, PhysicsEngine& physics, WindowManager& window);
    ~GameState();

    void step(const Uint8* keystate);

    Timeline& getTimeline();
    GameWorld& getGameWorld();
    PhysicsEngine& getPhysicsEngine();
    WindowManager& getWindowManager();
};