#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "init.h"
#include "Shape.h"
#include "Physics.h"
#include "InputHandler.h"
#include "Scaling.h"
#include "CollisionDetection.h"

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        return 1;
    }

    bool quit = false;
    bool proportionalScaling = false;
    float dx = 0, dy = 0;
    Physics physics;  // Manages gravity and other physics
    int windowWidth = INITIAL_WIDTH;
    int windowHeight = INITIAL_HEIGHT;

    Shape shape((windowWidth / 2), windowHeight - 50.0f, SQUARE_SIZE, SQUARE_SIZE, { 255, 0, 0, 255 }); // Red shape
    Shape shape1(200, windowHeight - 100, Rect_SIZE, SQUARE_SIZE, {0, 0, 0, 255}); // Black shape

    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    shape.setBounds(windowWidth, windowHeight);
    shape1.setBounds(windowWidth, windowHeight);  // Ensure shape1 is also set up correctly

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

                    // Update bounds and apply scaling to shapes
                    shape.setBounds(windowWidth, windowHeight);
                    shape1.setBounds(windowWidth, windowHeight);  // Ensure shape1 is also updated

                    float groundY = windowHeight - 50.0f;
                    shape.setY(groundY);


                    // Update shape1's position 
                    int newShape1X = (200);
                    int newShape1Y = (windowHeight - 100);
                    shape1.setX(newShape1X);
                    shape1.setY(newShape1Y);
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_TAB) {
                    proportionalScaling = !proportionalScaling;
                    shape.setProportionalScaling(proportionalScaling);
                    shape1.setProportionalScaling(proportionalScaling);
                }
            }
        }

        // Handle input and apply physics
        handleInput(quit, dx, dy);   // Get input for movement
        physics.applyGravity(dy);    // Apply gravity to vertical movement

        // Create SDL_Rect objects from the shapes
        SDL_Rect shapeRect = { shape.getX(), shape.getY(), shape.getWidth(), shape.getHeight() };
        SDL_Rect shape1Rect = { shape1.getX(), shape1.getY(), shape1.getWidth(), shape1.getHeight() };

        // Handle horizontal movement and check collision
        shape.move(static_cast<int>(dx), 0); // Move horizontally first
        shapeRect.x = shape.getX();  // Update position in the rect
        if (checkCollision(shapeRect, shape1Rect)) {
            // Collision on the horizontal axis, so undo the move
            if (dx > 0) {  // Moving right, so set position to the left of shape1
                shape.setX(shape1.getX() - shape.getWidth());
            }
            else if (dx < 0) {  // Moving left, so set position to the right of shape1
                shape.setX(shape1.getX() + shape1.getWidth());
            }
        }

        // Handle vertical movement and check collision
        shape.move(0, static_cast<int>(dy)); // Move vertically
        shapeRect.y = shape.getY();  // Update position in the rect
        if (checkCollision(shapeRect, shape1Rect)) {
            // Collision on the vertical axis, so undo the move
            if (dy > 0) {  // Falling down, so land on top of shape1
                shape.setY(shape1.getY() - shape.getHeight());
                dy = 0;  // Stop falling
                isOnGround = true;
            }
            else if (dy < 0) {  // Moving up, hitting the bottom of shape1
                shape.setY(shape1.getY() + shape1.getHeight());
                dy = 0;  // Stop upward movement
            }
        }

        // Check if the shape hits the ground
        float groundY = windowHeight - 50.0f;
        if (shape.getY() >= groundY) {
            shape.setY(groundY);  // Stop at the ground level
            dy = 0.0f;            // Reset vertical velocity
            isOnGround = true;     // Mark the shape as being on the ground
            isJumping = false;     // Reset jumping state
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
