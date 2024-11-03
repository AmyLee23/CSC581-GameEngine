#pragma once
#include <SDL.h>
#include <string>

class GameState;

class GameInput {
private:
    SDL_Scancode pause_key;
    SDL_Scancode scaling_key;
    SDL_Scancode quit_key;
    SDL_Scancode speedUp_key;
    SDL_Scancode speedDown_key;
    // New keys for dash input chord
    SDL_Scancode dash_key;
    SDL_Scancode left_key;
    SDL_Scancode right_key;
    std::string player_name; // player to speed up or slow down

    double adjustTimeScale(double currentScale, double adjustment, double minScale, double maxScale);

    // original global speed adjustment, currently unused
    void adjustGlobalTime(GameState& gameState, double adjustment);

public:
    GameInput();

    bool isDashChordActive() const;

    SDL_Scancode getPauseKey() const;
    SDL_Scancode getScalingKey() const;
    SDL_Scancode getQuitKey() const;
    SDL_Scancode getSpeedUpKey() const;
    SDL_Scancode getSpeedDownKey() const;
    const std::string& getPlayerName() const;

    void setPlayerName(const std::string& name);
    void setPauseKey(SDL_Scancode key);
    void setScalingKey(SDL_Scancode key);
    void setQuitKey(SDL_Scancode key);
    void setSpeedUpKey(SDL_Scancode key);
    void setSpeedDownKey(SDL_Scancode key);

    bool handleInput(const Uint8* keystate, GameState& gameState);
    std::pair<bool, int> handleInputFPS(const Uint8* keystate, GameState& gameState, int targetFps); // modifies frame rate when speeding up or slowing down

};