#include "init.h"
#include "SDL.h"

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Handle initialization error
        return false;
    }

    window = SDL_CreateWindow("SDL Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_RESIZABLE); // Initial size, can be adjusted
    if (!window) {
        // Handle window creation error
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        // Handle renderer creation error
        SDL_DestroyWindow(window);
        return false;
    }

    return true;
}

void getWindowSize(SDL_Window* window, int& width, int& height) {
    SDL_GetWindowSize(window, &width, &height);
}
