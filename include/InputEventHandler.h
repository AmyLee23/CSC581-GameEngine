#pragma once
#include "EventHandler.h"
#include "GameState.h"
#include "InputEvent.h"
#include <chrono>
#include <iostream>


class InputEventHandler : public EventHandler {
private:
    GameState& gameState;
    std::chrono::steady_clock::time_point lastLeftTap;
    std::chrono::steady_clock::time_point lastRightTap;
    bool leftKeyReleased = true;
    bool rightKeyReleased = true;
    const std::chrono::milliseconds doubleTapThreshold = std::chrono::milliseconds(300); // 300ms threshold

public:
    InputEventHandler(GameState& gs) : gameState(gs) {}

    void onEvent(std::shared_ptr<Event> event) override {
        auto inputEvent = std::static_pointer_cast<InputEvent>(event);
        NamedSprite& sprite = inputEvent->getSprite();
        auto* jumper = dynamic_cast<ControllableJumper*>(sprite.getMovement());

        const Uint8* keystate = inputEvent->getKeystate();

        if (jumper) {
            if (keystate[SDL_SCANCODE_LEFT]) {
                auto now = std::chrono::steady_clock::now();
                if (leftKeyReleased && now - lastLeftTap < doubleTapThreshold) {
                    std::cout << "Input chord double-tap detected for left key" << std::endl;
                    jumper->startDash(-1); // Start dash to the left
                }
                lastLeftTap = now;
                leftKeyReleased = false;
            }
            else {
                leftKeyReleased = true;
            }

            if (keystate[SDL_SCANCODE_RIGHT]) {
                auto now = std::chrono::steady_clock::now();
                if (rightKeyReleased && now - lastRightTap < doubleTapThreshold) {
                    std::cout << "Input chord double-tap detected for right key" << std::endl;
                    jumper->startDash(1); // Start dash to the right
                }
                lastRightTap = now;
                rightKeyReleased = false;
            }
            else {
                rightKeyReleased = true;
            }
        }
        // Handle regular movement input (no dash)
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