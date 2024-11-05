#pragma once
#include "Event.h"
#include "NamedSprite.h"
#include <SDL.h>

class InputEvent : public Event {
private:
    NamedSprite& sprite;
    const Uint8* keystate;
    GameState& gameState;
    double dt;

public:
    InputEvent(Timeline& timeline,
        NamedSprite& sprite,
        const Uint8* keystate,
        GameState& gameState,
        double dt)
        : Event(timeline, "Input", 4), // Highest priority for responsive controls
        sprite(sprite),
        keystate(keystate),
        gameState(gameState),
        dt(dt) {}

    NamedSprite& getSprite() const { return sprite; }
    const Uint8* getKeystate() const { return keystate; }
    GameState& getGameState() const { return gameState; }
    double getDt() const { return dt; }
};