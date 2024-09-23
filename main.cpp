#include "WindowManager.h"
#include "Game.h"
#include "ControllableShape.h"
#include "FixedShape.h"
#include "MovingShape.h"
#include "PhysicsEngine.h"
#include "Drawer.h"

int main(int argc, char* args[]) {
    WindowManager windowManager;
    PhysicsEngine physics(windowManager);
    Game game(windowManager, physics);
    Drawer drawer;

    // WindowManager.init() if block

    // create shapes, change configs, add to game
    // for controllable shapes, dy must be zero if flying is false
    // make sure gravity, jump velocity, and initial velocities are appropriate as they are pseudo realistic in terms of m/s
    // can also adjust pixels per meter

    // get keystate
    // SDL_Event e;

    // while (!quit)
        // while (SDL_PollEvent(&e) != 0) {
            // handle SDL quit etc

            // handle window resize events and update WindowManager with updateWindowSize (not setWindowSize())

            // handle a key toggle for scaling like:
            //if (e.type == SDL_KEYDOWN) {
            //    if (e.key.keysym.sym == SDLK_a) {
            //        game.toggleScalingMode();
            //    }
            //}
           
        // game.step(keystate)
        // update window with WindowManager

    // close/destroy window with WindowManager
    return 0;
}