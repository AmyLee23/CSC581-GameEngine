#include "WindowManager.h"
#include "Game.h"

int main(int argc, char* args[]) {
    WindowManager windowManager;
    Game game;

    if (!windowManager.init(1920, 1080)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update the window with the current game state
        windowManager.updateWindow(game);
    }

    windowManager.close();  // Clean up the window and renderer
    return 0;
}
