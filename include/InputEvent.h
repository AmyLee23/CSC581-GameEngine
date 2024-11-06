#pragma once
#include "Event.h"
#include "NamedSprite.h"
#include <SDL.h>

class InputEvent : public Event {
public:
    InputEvent(Timeline& timeline,
        NamedSprite& sprite,
        const Uint8* keystate,
        GameState& gameState,
        double dt);

    NamedSprite& getSprite() const;
    const Uint8* getKeystate() const;
    GameState& getGameState() const;
    double getDt() const;

private:
    NamedSprite& sprite;
    const Uint8* keystate;
    GameState& gameState;
    double dt;
};