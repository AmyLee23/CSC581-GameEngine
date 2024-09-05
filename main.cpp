#include <SDL.h>  // Include the SDL header for graphics and window management
#include <stdio.h> // Include standard I/O functions
#include <iostream> // Include standard I/O stream operations
#include "init.h" // Custom header for initialization functions
#include "Shape.h" // Custom header for shape-related operations
#include "Physics.h" // Custom header for physics calculations
#include "InputHandler.h" // Custom header for handling user input
#include "Scaling.h" // Custom header for scaling operations
#include "CollisionDetection.h" // Custom header for collision detection

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr; // Pointer to the SDL window
    SDL_Renderer* renderer = nullptr; // Pointer to the SDL renderer

    // Initialize SDL and create a window and renderer
    if (!init(window, renderer)) {
        return 1; // Exit if initialization fails
    }

    bool quit = false; // Flag to control the main loop
    bool proportionalScaling = false; // Flag to toggle proportional scaling of shapes
    float dx = 0, dy = 0; // Movement variables for shapes
    Physics physics;  // Create a Physics object to manage gravity and other physics
    int windowWidth = INITIAL_WIDTH; // Initialize window width
    int windowHeight = INITIAL_HEIGHT; // Initialize window height

    // Create two shapes with specified properties and colors
    Shape shape((windowWidth / 2), windowHeight - 50.0f, SQUARE_SIZE, SQUARE_SIZE, { 255, 0, 0, 255 }); // Red shape
    Shape shape1(windowWidth / 3, windowHeight - 100, Rect_SIZE, SQUARE_SIZE, { 0, 0, 0, 255 }); // Black shape

    // Get the current window size
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    shape.setBounds(windowWidth, windowHeight); // Set bounds for shape
    shape1.setBounds(windowWidth, windowHeight); // Set bounds for shape1

    while (!quit) { // Main loop
        SDL_Event e; // SDL event to handle user input and window events

        // Poll for events
        while (SDL_PollEvent(&e) != 0) {
            // Handle quit event
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // Handle window resize event
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    // Update window size and logical size of the renderer
                    windowWidth = e.window.data1;
                    windowHeight = e.window.data2;
                    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

                    // Update shape bounds and apply scaling
                    shape.setBounds(windowWidth, windowHeight);
                    shape1.setBounds(windowWidth, windowHeight);

                    // Adjust shape positions according to the new window size
                    float groundY = windowHeight - 50.0f;
                    shape.setY(groundY);


                    //Update shape1 position based on window size
                    int newShape1X = (windowWidth / 3);
                    int newShape1Y = (windowHeight - 100);
                    shape1.setX(newShape1X);
                    shape1.setY(newShape1Y);
                }
            }
            // Handle key press events so if the tab is press, they change to proportional
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_TAB) {
                    // Toggle proportional scaling for shapes
                    proportionalScaling = !proportionalScaling;
                    shape.setProportionalScaling(proportionalScaling);
                    shape1.setProportionalScaling(proportionalScaling);
                }
            }
        }

        // Handle input and apply physics
        handleInput(quit, dx, dy); // Get input for movement (e.g., keyboard input)
        physics.applyGravity(dy); // Apply gravity to the vertical movement of the shape

        // Create SDL_Rect objects for shape collision detection
        SDL_Rect shapeRect = { shape.getX(), shape.getY(), shape.getWidth(), shape.getHeight() };
        SDL_Rect shape1Rect = { shape1.getX(), shape1.getY(), shape1.getWidth(), shape1.getHeight() };

        // Handle horizontal movement and check for collisions
        shape.move(static_cast<int>(dx), 0); // Move shape horizontally
        shapeRect.x = shape.getX(); // Update the x-coordinate in the rectangle
        if (checkCollision(shapeRect, shape1Rect)) {
            // Collision detected on the horizontal axis; undo the move
            if (dx > 0) {  // Moving right, so move shape to the left of shape1
                shape.setX(shape1.getX() - shape.getWidth());
            }
            else if (dx < 0) {  // Moving left, so move shape to the right of shape1
                shape.setX(shape1.getX() + shape1.getWidth());
            }
        }

        // Handle vertical movement and check for collisions
        shape.move(0, static_cast<int>(dy)); // Move shape vertically
        shapeRect.y = shape.getY(); // Update the y-coordinate in the rectangle
        if (checkCollision(shapeRect, shape1Rect)) {
            // Collision detected on the vertical axis; undo the move
            if (dy > 0) {  // Falling down, so move shape to land on top of shape1
                shape.setY(shape1.getY() - shape.getHeight());
                dy = 0; // Stop falling
                isOnGround = true; // Mark shape as being on the ground
            }
            else if (dy < 0) {  // Moving up, hitting the bottom of shape1
                shape.setY(shape1.getY() + shape1.getHeight());
                dy = 0; // Stop upward movement
            }
        }

        // Check if the shape hits the ground
        float groundY = windowHeight - 50.0f;
        if (shape.getY() >= groundY) {
            shape.setY(groundY); // Stop at the ground level
            dy = 0.0f; // Reset vertical velocity
            isOnGround = true; // Mark the shape as being on the ground
            isJumping = false; // Reset jumping state
        }

        // Clear screen with a blue background
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Set color to blue
        SDL_RenderClear(renderer);

        // Draw shapes
        shape.draw(renderer); // Draw the first shape
        shape1.draw(renderer); // Draw the second shape

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Reset horizontal velocity for the next frame
        dx = 0;
    }

    // Clean up and close SDL
    SDL_DestroyRenderer(renderer); // Destroy the renderer
    SDL_DestroyWindow(window); // Destroy the window
    SDL_Quit(); // Quit SDL

    return 0; // Exit program
}
