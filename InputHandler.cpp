#include "InputHandler.h"
#include <SDL.h>

void handleInput(bool& quit, float& dx, float& dy) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) dy -= 2.0f;
    if (state[SDL_SCANCODE_DOWN]) dy += 1.0f;
    if (state[SDL_SCANCODE_LEFT]) dx -= 1.0f;
    if (state[SDL_SCANCODE_RIGHT]) dx += 1.0f;
    if (state[SDL_SCANCODE_ESCAPE]) quit = true;
}
