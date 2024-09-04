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

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    bool quit = false;
    float dx = 0, dy = 0;
    Physics physics;  // Manages gravity and other physics
    bool proportionalMode = true;
    Shape shape(0, windowHeight-0.1f, SQUARE_SIZE, SQUARE_SIZE, proportionalMode, { 255, 0, 0, 255 }); // Red shape
    Shape shape1(windowWidth/3, windowHeight/2, 200, 50, proportionalMode, { 0, 0, 0, 255 }); // Black shape
    shape.setBounds(windowWidth, windowHeight);
    shape1.setBounds(windowWidth, windowHeight);

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
                    std::cout << windowWidth << " " << windowHeight;
                    shape1.setBounds(windowWidth, windowHeight);
                }
            }
        }

        // Handle input and apply physics
        handleInput(quit, dx, dy);   // Get input for movement
        shape.proportionality(proportionalMode);
        shape1.proportionality(proportionalMode);
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
