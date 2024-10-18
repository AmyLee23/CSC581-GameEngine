#include "WindowManager.h"
#include "PhysicsEngine.h"
#include "GameWorld.h"
#include "Timeline.h"
#include "GameState.h"
#include "NamedSprite.h"
#include "StandardPosition.h"
#include "BouncingMovement.h"
#include "ControllableFlyer.h"
#include "SimpleRectDrawer.h"
#include "ControllableJumper.h"
#include "MovementComponent.h"
#include "GameInput.h"

#include <thread>
#include <fstream>
#include <json.hpp>

int main(int argc, char* args[]) {
    // Load sprite data from JSON
    std::ifstream inputFile("initial_game_state.json");
    nlohmann::json jsonData;
    inputFile >> jsonData;

    // game initialization
    WindowManager windowManager;
    PhysicsEngine physics;
    Timeline globalTimeline(nullptr, 1);
    GameWorld gameWorld(800, 600);
    GameState gameState(globalTimeline, gameWorld, physics, windowManager);

    // other configs
    gameState.getPhysicsEngine().setPixelsPerMeter(16.0);
    gameState.getPhysicsEngine().setGravity(5.0 / 100.0);
    GameInput gameInput;

    // create window
    if (!windowManager.init(900, 700)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    // Iterate through the sprite data and create the sprites
    for (const auto& spriteData : jsonData["sprites"]) {
        std::string name = spriteData["name"];
        SDL_Color color = {
            spriteData["color"]["r"],
            spriteData["color"]["g"],
            spriteData["color"]["b"],
            spriteData["color"]["a"]
        };
        PositionComponent* position = new StandardPosition(
            spriteData["position"]["x"],
            spriteData["position"]["y"],
            spriteData["position"]["width"],
            spriteData["position"]["height"]
        );
        DrawerComponent* drawer = new SimpleRectDrawer(color);

        // Initialize the sprite's movement component
        MovementComponent* movement = nullptr;
        InputComponent* input = nullptr;

        if (name == "jumper") {
            // Controllable jumper movement
            ControllableJumper* jumperMovement = new ControllableJumper(45.0 / 100.0, 0.0, 1.5);
            movement = jumperMovement;
            input = jumperMovement;  // Assign input component to handle user input
            gameInput.setPlayerName(name);
        }
        else if (name.find("platform") != std::string::npos) {
            // Create a platform movement component with parameters from JSON
            float bounceSpeed = spriteData["movementParams"]["bounceSpeed"];
            movement = new BouncingMovement(bounceSpeed / 100.0, 0); // Set movement speed based on JSON
        }

        // Create the NamedSprite with the appropriate movement component
        Timeline* localTimeline = new Timeline(&globalTimeline, spriteData["timeline"]);
        NamedSprite* sprite = new NamedSprite(name, position, movement, input, drawer, *localTimeline);
        gameWorld.addObject(sprite);
    }

    gameInput.setPlayerName("jumper"); // Ensure this is set

    bool quit = false;
    SDL_Event e;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    int targetFPS = 120;
    int64_t timePerFrame = 1000 / targetFPS;

    while (!quit) {
        int64_t frameStartTime = globalTimeline.getTime();

        while (SDL_PollEvent(&e) != 0) {
            // SDL quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // handle game input
            else if (e.type == SDL_KEYDOWN) {
                quit = gameInput.handleInput(keystate, gameState);
            }
            // window resize
            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int windowWidth = e.window.data1;
                    int windowHeight = e.window.data2;
                    windowManager.updateWindowSize(windowWidth, windowHeight, gameState);
                }
            }
        }
        gameState.step(keystate);
        windowManager.updateWindow(gameState);

        int64_t frameEndTime = globalTimeline.getTime();
        int64_t frameDuration = frameEndTime - frameStartTime;
        int64_t sleepTime = timePerFrame - frameDuration;

        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }

    windowManager.close();
    return 0;
}
