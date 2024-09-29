#include "GameInput.h"
#include "GameState.h"
#include "WindowManager.h"
#include "Timeline.h"
#include "GameWorld.h"
#include "NamedSprite.h"

GameInput::GameInput()
    : pause_key(SDL_SCANCODE_P),
    scaling_key(SDL_SCANCODE_S),
    quit_key(SDL_SCANCODE_Q),
    speedUp_key(SDL_SCANCODE_EQUALS),
    speedDown_key(SDL_SCANCODE_MINUS) {}

SDL_Scancode GameInput::getPauseKey() const { return pause_key; }
SDL_Scancode GameInput::getScalingKey() const { return scaling_key; }
SDL_Scancode GameInput::getQuitKey() const { return quit_key; }
SDL_Scancode GameInput::getSpeedUpKey() const { return speedUp_key; }
SDL_Scancode GameInput::getSpeedDownKey() const { return speedDown_key; }

void GameInput::setPauseKey(SDL_Scancode key) { pause_key = key; }
void GameInput::setScalingKey(SDL_Scancode key) { scaling_key = key; }
void GameInput::setQuitKey(SDL_Scancode key) { quit_key = key; }
void GameInput::setSpeedUpKey(SDL_Scancode key) { speedUp_key = key; }
void GameInput::setSpeedDownKey(SDL_Scancode key) { speedDown_key = key; }

double GameInput::adjustTimeScale(double currentScale, double adjustment, double minScale, double maxScale) {
    double newScale = currentScale + adjustment;
    if (newScale > maxScale) {
        return maxScale;
    }
    if (newScale < minScale) {
        return minScale;
    }
    return newScale;
}

bool GameInput::handleInput(const Uint8* keystate, GameState& gameState) {
    bool quit = false;
    if (keystate[quit_key]) {
        quit = true;
    }
    if (keystate[scaling_key]) {
        gameState.getWindowManager().toggleProportionalMode();
    }
    if (keystate[pause_key]) {
        Timeline& globalTimeline = gameState.getTimeline();
        if (globalTimeline.isPaused()) {
            globalTimeline.unpause();
        }
        else {
            globalTimeline.pause();
        }
    }
    double adjustment = 0.0;
    if (keystate[speedUp_key]) {
        adjustment = 0.1;
    }
    else if (keystate[speedDown_key]) {
        adjustment = -0.1;
    }
    if (adjustment != 0.0) {
        Timeline& globalTimeline = gameState.getTimeline();
        GameWorld& gameWorld = gameState.getGameWorld();
        double newGlobalTimeScale = adjustTimeScale(globalTimeline.getTimeScale(), adjustment, 0.5, 2.0);
        globalTimeline.setTimeScale(newGlobalTimeScale);
        for (auto& pair : gameWorld.getObjects()) {
            NamedSprite* sprite = pair.second;
            Timeline& spriteTimeline = sprite->getTimeline();
            double newSpriteTimeScale = adjustTimeScale(spriteTimeline.getTimeScale(), adjustment, 0.5, 2.0);
            spriteTimeline.setTimeScale(newSpriteTimeScale);
        }
    }
    return quit;
}
