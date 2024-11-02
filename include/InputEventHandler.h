#pragma once
#include "EventHandler.h"
#include "GameState.h"
#include "InputEvent.h"
class InputEventHandler : public EventHandler {
private:
    GameState& gameState;

public:
    InputEventHandler(GameState& gs) : gameState(gs) {}

    void onEvent(std::shared_ptr<Event> event) override {
        auto inputEvent = std::static_pointer_cast<InputEvent>(event);
        NamedSprite& sprite = inputEvent->getSprite();

        // Use your existing input handling through InputComponent
        if (InputComponent* inputComponent = sprite.getInput()) {
            inputComponent->handleInput(
                inputEvent->getKeystate(),
                sprite,  
                inputEvent->getGameState(),
                inputEvent->getDt()
            );
        }
    }
};