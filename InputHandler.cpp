#include "InputHandler.h"
#include <SDL.h>
#include <stdio.h>

// Constants for gravity and jump
const float GRAVITY = 0.5f;
const float JUMP_VELOCITY = -12.0f;

// Variables for tracking the character's state
bool isJumping = false;
bool isOnGround = true;

void handleInput(bool& quit, float& dx, float& dy) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Handle horizontal movement
    if (state[SDL_SCANCODE_LEFT]) dx -= 1.0f;
    if (state[SDL_SCANCODE_RIGHT]) dx += 1.0f;

    // Handle jumping
    if (state[SDL_SCANCODE_SPACE] && isOnGround) {
        dy = JUMP_VELOCITY;  // Start jumping by setting the vertical velocity
        isJumping = true;
        isOnGround = false;
    }

    // Handle quit
    if (state[SDL_SCANCODE_ESCAPE]) quit = true;
}

void applyGravity(float& dy) {
    // Apply gravity if the character is not on the ground
    if (!isOnGround) {
        dy += GRAVITY;  // Increase the downward velocity
    }
}
