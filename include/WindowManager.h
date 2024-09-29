#pragma once

#include <SDL.h>
#include <iostream>
#include "GameState.h"

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    bool init(int screen_width = 800, int screen_height = 600);
    void updateWindow(GameState& gameState);
    void close();

    bool windowExists() const;

    int getWindowWidth() const;
    int getWindowHeight() const;
    int getLastWindowWidth() const;
    int getLastWindowHeight() const;
    void setWindowSize(int width, int height);
    void updateWindowSize(int width, int height, GameState& gameState);

    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();

    bool getProportionalMode() const;
    void setProportionalMode(bool mode);
    void toggleProportionalMode();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowWidth;
    int windowHeight;
    int lastWindowWidth;
    int lastWindowHeight;
    bool proportionalMode;
    bool m_windowExists;
};
