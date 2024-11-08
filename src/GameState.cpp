#include "pch.h"
#include "GameState.h"
#include "Timeline.h"
#include "GameWorld.h"
#include "PhysicsEngine.h"
#include "WindowManager.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "GameTypes.h"
#include "EventManager.h"
#include "InputEvent.h"
#include <iostream>

GameState::GameState(Timeline& timeline, GameWorld& world, PhysicsEngine& physics, WindowManager& window, GameTypes& types)
    : globalTimeline(timeline), gameWorld(world), physicsEngine(physics), windowManager(window), gameTypes(types), eventManager(timeline), client(nullptr) {}

GameState::GameState(Timeline& timeline, GameWorld& world, PhysicsEngine& physics, WindowManager& window, GameTypes& types, JClient& client)
    : globalTimeline(timeline), gameWorld(world), physicsEngine(physics), windowManager(window), gameTypes(types), eventManager(timeline), client(&client) {}

GameState::~GameState() {
    for (auto& pair : gameWorld.getObjects()) {
        delete pair.second;
    }
}

void GameState::step(const Uint8* keystate) {
    stepInput(keystate);
    stepMovement();
    eventManager.update();
}

void GameState::stepInput(const Uint8* keystate) {
    // Handle input for each NamedSprite
    for (const auto& pair : gameWorld.getObjects()) {
        NamedSprite* sprite = pair.second;

        // Handle input if the sprite has an InputComponent
        if (InputComponent* inputComponent = sprite->getInput()) {
            double dt = sprite->getTimeline().getInputDt();
            queueEvent(
                std::make_shared<InputEvent>(
                    globalTimeline,
                    *sprite,
                    keystate,
                    *this,
                    dt
                )
            );
        }
    }
}

void GameState::stepMovement() {
    // Handle movement for each NamedSprite
    for (const auto& pair : gameWorld.getObjects()) {
        NamedSprite* sprite = pair.second;

        // Handle movement if the sprite has a MovementComponent
        if (MovementComponent* movementComponent = sprite->getMovement()) {
            double dt = sprite->getTimeline().getMovementDt();
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

GameTypes& GameState::getGameTypes() {
    return gameTypes;
}

void GameState::queueEvent(std::shared_ptr<Event> event) {
    pendingEvents.push(event);
}

void GameState::processPendingEvents() {
    while (!pendingEvents.empty()) {
        auto event = pendingEvents.front();
        pendingEvents.pop();

        if (event->getType() == "Death" && client) {
            NamedSprite* sprite = gameWorld.getObjectByName(client->getPlayerName());
            std::pair<double, double> currentPos = { sprite->getPosition()->getX(), sprite->getPosition()->getY()};
            client->sendInput(client->getPlayerName(), gameTypes.getTypeHash("player"), currentPos, true);
            std::cout << "Sending death event across network" << std::endl;
        }
        else {
            eventManager.raiseEvent(event);
        } 
    }
}
