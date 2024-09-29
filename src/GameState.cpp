#include "pch.h"
#include "GameState.h"
#include "Timeline.h"
#include "GameWorld.h"
#include "PhysicsEngine.h"
#include "WindowManager.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include <iostream>

GameState::GameState(Timeline& timeline, GameWorld& world, PhysicsEngine& physics, WindowManager& window)
    : globalTimeline(timeline), gameWorld(world), physicsEngine(physics), windowManager(window) {}

GameState::~GameState() {
    for (auto& pair : gameWorld.getObjects()) {
        delete pair.second;
    }
}

void GameState::step(const Uint8* keystate) {
    // Handle input and movement for each NamedSprite
    for (const auto& pair : gameWorld.getObjects()) {
        NamedSprite* sprite = pair.second;

        // Get dt from the sprite's local timeline
        double dt = sprite->getTimeline().getDt();

        //std::cout << "Delta time (dt): " << dt << std::endl;

        // Handle input
        if (InputComponent* inputComponent = sprite->getInput()) {
            inputComponent->handleInput(keystate, *sprite, *this, dt);
        }

        // Handle movement
        if (MovementComponent* movementComponent = sprite->getMovement()) {
            movementComponent->move(*sprite, *this, dt);
        }
    }
}

Timeline& GameState::getTimeline() {
    return globalTimeline;
}

GameWorld& GameState::getGameWorld() {
    return gameWorld;
}

PhysicsEngine& GameState::getPhysicsEngine() {
    return physicsEngine;
}

WindowManager& GameState::getWindowManager() {
    return windowManager;
}
