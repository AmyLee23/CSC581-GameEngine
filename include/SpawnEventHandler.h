#pragma once
#include "EventHandler.h"
#include "SpawnEvent.h"

class SpawnEventHandler : public EventHandler {
public:
    void onEvent(std::shared_ptr<Event> event) override {
        auto spawnEvent = std::static_pointer_cast<SpawnEvent>(event);
        auto& sprite = spawnEvent->getSprite();
        // Reset position to spawn point stored in sprite
        sprite.getPosition()->setX(sprite.getSpawnX());
        sprite.getPosition()->setY(sprite.getSpawnY());

        // Reset any movement
        if (auto movement = sprite.getMovement()) {
            movement->setDy(0);
        }
    }
};