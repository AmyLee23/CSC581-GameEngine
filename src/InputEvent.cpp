#include "pch.h"
#include "InputEvent.h"
#include "GameState.h"  // Include here to resolve the forward declaration
#include "Timeline.h"

InputEvent::InputEvent(Timeline& timeline,
    NamedSprite& sprite,
    const Uint8* keystate,
    GameState& gameState,
    double dt)
    : Event(timeline, "Input", 4),  // Set type to "Input" and priority to 4
    sprite(sprite),
    keystate(keystate),
    gameState(gameState),
    dt(dt) {}

NamedSprite& InputEvent::getSprite() const {
    return sprite;
}

const Uint8* InputEvent::getKeystate() const {
    return keystate;
}

GameState& InputEvent::getGameState() const {
    return gameState;
}

double InputEvent::getDt() const {
    return dt;
}
