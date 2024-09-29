#pragma once
#include <SDL.h>

class GameState;

class GameInput {
private:
    SDL_Scancode pause_key;
    SDL_Scancode scaling_key;
    SDL_Scancode quit_key;
    SDL_Scancode speedUp_key;
    SDL_Scancode speedDown_key;
    double adjustTimeScale(double currentScale, double adjustment, double minScale, double maxScale);

public:
    GameInput();

    SDL_Scancode getPauseKey() const;
    SDL_Scancode getScalingKey() const;
    SDL_Scancode getQuitKey() const;
    SDL_Scancode getSpeedUpKey() const;
    SDL_Scancode getSpeedDownKey() const;
    void setPauseKey(SDL_Scancode key);
    void setScalingKey(SDL_Scancode key);
    void setQuitKey(SDL_Scancode key);
    void setSpeedUpKey(SDL_Scancode key);
    void setSpeedDownKey(SDL_Scancode key);

    bool handleInput(const Uint8* keystate, GameState& gameState);
};
