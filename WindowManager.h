#pragma once

#include <SDL.h>
#include <iostream>

#include "Game.h"

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    bool init(int screen_width = 800, int screen_height = 600);  // init SDL window and renderer with optional width and height
    void updateWindow(Game& game);  // update window with the current game state
    void close();                   // close SDL window and renderer, release all resources

    bool windowExists() const;      // returns whether the window is created

    int getWindowWidth() const;
    int getWindowHeight() const;
    void setWindowSize(int width, int height);

private:
    SDL_Window* window;             // SDL window pointer
    SDL_Renderer* renderer;         // SDL renderer pointer
    int windowWidth;
    int windowHeight;
    bool m_windowExists;            // if window exists
};
