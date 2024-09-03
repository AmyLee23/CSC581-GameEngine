#include <SDL.h>
#include <iostream>
#include "init.h"
#include "Shape.h"
#include "Physics.h"
#include "InputHandler.h"
#include "Scaling.h"
#include "CollisionDetection.h"

// Constants for ground level
const float GROUND_Y = SCREEN_HEIGHT-50.0f;  // Example value, adjust based on window height

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return 1;
    }

    bool quit = false;
    float dx = 0, dy = 0;
    Physics physics;  // Manages gravity and other physics
    Shape shape(100, GROUND_Y, SQUARE_SIZE, SQUARE_SIZE, { 255, 0, 0, 255 }); // Red shape
    Shape shape1(SCREEN_WIDTH/3, SCREEN_HEIGHT/2, 200, 50, { 0, 0, 0, 255 }); // Black shape

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

        // Handle input and apply physics
        handleInput(quit, dx, dy);   // Get input for movement
        physics.applyGravity(dy);    // Apply gravity to vertical movement

        // Update shape position
        shape.move(static_cast<int>(dx), static_cast<int>(dy));

        // Check if the shape hits the ground
        if (shape.getY() >= GROUND_Y) {
            shape.setY(GROUND_Y);  // Stop at the ground level
            dy = 0.0f;             // Reset vertical velocity
            isOnGround = true;      // Mark the shape as being on the ground
            isJumping = false;      // Reset jumping state
        }

        // Clear screen and render
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue background
        SDL_RenderClear(renderer);

        // Draw shapes
        shape.draw(renderer);
        shape1.draw(renderer);

        SDL_RenderPresent(renderer);

        // Reset horizontal velocity after each frame
        dx = 0;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
