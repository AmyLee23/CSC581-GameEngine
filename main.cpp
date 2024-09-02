#include <SDL.h>
#include <iostream>
#include "init.h"
#include "Shape.h"
#include "Physics.h"
#include "InputHandler.h"

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return 1;
    }

    bool quit = false;
    float dx = 0, dy = 0;
    Physics physics;
    Shape shape(100, 100, SQUARE_SIZE, SQUARE_SIZE, { 255, 0, 0, 255 }); // Red shape

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    shape.setBounds(windowWidth, windowHeight);

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    windowWidth = e.window.data1;
                    windowHeight = e.window.data2;
                    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
                    shape.setBounds(windowWidth, windowHeight);
                }
            }
        }

        handleInput(quit, dx, dy);
        physics.applyGravity(dy);
        shape.move(static_cast<int>(dx), static_cast<int>(dy));

        // Clear screen and render
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue background
        SDL_RenderClear(renderer);

        // Draw shape
        shape.draw(renderer);

        SDL_RenderPresent(renderer);

        dx = 0;
        dy = 0;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
