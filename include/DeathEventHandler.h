#pragma once
#include "EventHandler.h"
#include "GameState.h"
#include "DeathEvent.h"
#include "SpawnEvent.h"

class DeathEventHandler : public EventHandler {
private:
    GameState& gameState;

public:
    DeathEventHandler(GameState& gs) : gameState(gs) {}

    void onEvent(std::shared_ptr<Event> event) override {
        auto deathEvent = std::static_pointer_cast<DeathEvent>(event);
        auto& sprite = deathEvent->getSprite();
        // Raise a SpawnEvent to respawn the player
        gameState.queueEvent(
            std::make_shared<SpawnEvent>(
                sprite.getTimeline(),
                sprite
            )
        );
    }
};