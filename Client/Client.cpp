#include "JClient.h"
#include "GameState.h"
#include "Timeline.h"
#include "PhysicsEngine.h"
#include "WindowManager.h"
#include "NamedSprite.h"
#include "StandardPosition.h"
#include "ControllableJumper.h"
#include "SimpleRectDrawer.h"
#include "GameWorld.h"
#include "GameInput.h"
#include <SDL.h>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <set>

// Synchronization primitives
std::mutex mtx;

// Thread function to handle input
void handleInput(GameState& gameState, const Uint8* keystate) {
    std::unique_lock<std::mutex> lock(mtx);
    gameState.stepInput(keystate);
}

// Thread function to handle movement
void handleMovement(GameState& gameState) {
    std::unique_lock<std::mutex> lock(mtx);
    gameState.stepMovement();
}

int main(int argc, char* args[]) {
    // Initialize the client (server IP, port REQ-REP)
    JClient client("127.0.0.1", 5555);

    // Ask the server for the player number and add client (e.g., PlayerX)
    client.connectToServer();
    SDL_Color blue = { 0, 0, 0, 255 };

    // Initialize game engine components
    Timeline globalTimeline(nullptr, 1);
    PhysicsEngine physics;
    WindowManager windowManager;
    GameWorld gameWorld(800, 600);
    GameState gameState(globalTimeline, gameWorld, physics, windowManager);
    GameInput gameInput;

    // Configure the physics engine
    gameState.getPhysicsEngine().setPixelsPerMeter(16.0);
    gameState.getPhysicsEngine().setGravity(5.0 / 100.0);

    // colors
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color red = { 255, 0, 0, 255 };

    // Create the fixed shape (must match server's)
    PositionComponent* fixedPos = new StandardPosition(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
    DrawerComponent* fixedDrawer = new SimpleRectDrawer(green);
    Timeline fixedTimeline(&globalTimeline, 64);
    NamedSprite* fixedShape = new NamedSprite("ground", fixedPos, nullptr, nullptr, fixedDrawer, fixedTimeline);
    gameWorld.addObject(fixedShape);

    // Fixed shape (block)
    PositionComponent* blockPos = new StandardPosition(100.0, static_cast<double>(gameWorld.getWorldHeight() - 75), 600, 25);
    DrawerComponent* blockDrawer = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline blockTimeline(&globalTimeline, 64);
    NamedSprite* block = new NamedSprite("block", blockPos, nullptr, nullptr, blockDrawer, blockTimeline);
    gameWorld.addObject(block);

    // Fixed shape (pillar 1)
    PositionComponent* groundPos1 = new StandardPosition(100.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 50, 150);
    DrawerComponent* groundDrawer1 = new SimpleRectDrawer({ 0, 0, 0, 255 });
    Timeline groundTimeline1(&globalTimeline, 64);
    NamedSprite* pillar1 = new NamedSprite("pillar1", groundPos1, nullptr, nullptr, groundDrawer1, groundTimeline1);
    gameWorld.addObject(pillar1);

    // Fixed shape (pillar 2)
    PositionComponent* groundPos2 = new StandardPosition(200.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 50, 180);
    DrawerComponent* groundDrawer2 = new SimpleRectDrawer({ 0, 0, 0, 255 });
    Timeline groundTimeline2(&globalTimeline, 64);
    NamedSprite* pillar2 = new NamedSprite("pillar2", groundPos2, nullptr, nullptr, groundDrawer2, groundTimeline2);
    gameWorld.addObject(pillar2);

    // Fixed shape (pillar 3)
    PositionComponent* groundPos3 = new StandardPosition(600.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 50, 210);
    DrawerComponent* groundDrawer3 = new SimpleRectDrawer({ 0, 0, 0, 255 });
    Timeline groundTimeline3(&globalTimeline, 64);
    NamedSprite* pillar3 = new NamedSprite("pillar3", groundPos3, nullptr, nullptr, groundDrawer3, groundTimeline3);
    gameWorld.addObject(pillar3);

    // Fixed shape (pillar 4)
    PositionComponent* groundPos4 = new StandardPosition(700.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 50, 240);
    DrawerComponent* groundDrawer4 = new SimpleRectDrawer({0, 0, 0, 255});
    Timeline groundTimeline4(&globalTimeline, 64);
    NamedSprite* pillar4 = new NamedSprite("pillar4", groundPos4, nullptr, nullptr, groundDrawer4, groundTimeline4);
    gameWorld.addObject(pillar4);

    // Create the ControllableJumper(player object) for the client
    std::string name = client.getPlayerName();
    gameInput.setPlayerName(name);
    int playerNum = std::stoi(name.substr(6));
    PositionComponent* jumperPos = new StandardPosition(50.0, 0.0 + playerNum * 50, 50, 50);
    ControllableJumper* jumperMovement = new ControllableJumper(45.0 / 100.0, 0.0, 1.5);  // Handles movement and input
    DrawerComponent* jumperDrawer = new SimpleRectDrawer(red);
    Timeline jumperTimeline(&globalTimeline, 10);
    NamedSprite* jumper = new NamedSprite(name, jumperPos, jumperMovement, jumperMovement, jumperDrawer, jumperTimeline);
    gameWorld.addObject(jumper);
    //std::cout << "Initialized player at (" << 50.0 << ", " << 0.0 + playerNum * 50 << ")" << std::endl;

    // Initialize SDL for rendering
    if (!windowManager.init(800, 600)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    // Main client loop
    bool quit = false;
    SDL_Event e;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    int targetFPS = 30;  // Frame rate target
    int localFPS = targetFPS;
    int64_t timePerFrame = 1000 / targetFPS;  // Milliseconds per frame
    std::unordered_map<std::string, std::pair<double, double>> movingSprites;

    while (!quit) {
        // Start frame timing
        int64_t frameStartTime = globalTimeline.getTime();

        // If there are new shapes in the server's game state, add them as fixed shapes
        for (const auto& spriteData : movingSprites) {
            const std::string& name = spriteData.first;
            const std::pair<double, double>& position = spriteData.second;

            NamedSprite* sprite = gameWorld.getObjectByName(name);
            if (!sprite) {
                // Determine the drawer color based on the name
                //uncommentSDL_Color drawerColor = (name.find("Player") != std::string::npos) ? red : green;
                if (position.first == -1.0) {
                    continue;
                }
                SDL_Color drawerColor;
                if (name.find("Player") != std::string::npos) {
                    drawerColor = red;
                }
                else if (name.find("block") != std::string::npos) {
                    drawerColor = { 150, 75, 0, 255 };
                }
                else if (name.find("pillar") != std::string::npos) {
                    drawerColor = {0, 0, 0, 255};
                }
                else {
                    drawerColor = green;
                }

                // If the object doesn't exist, create it as a fixed shape
                PositionComponent* fixedPos = new StandardPosition(position.first, position.second, 50, 50);
                DrawerComponent* fixedDrawer = new SimpleRectDrawer(drawerColor);
                NamedSprite* newSprite = new NamedSprite(name, fixedPos, nullptr, nullptr, fixedDrawer, globalTimeline);
                gameWorld.addObject(newSprite);
                std::cout << "Added " << name << " as a fixed shape to the game world." << std::endl;
            }
            else {
                // Update the position of existing moving shapes
                if (position.first == -1.0) {
                    std::cout << "removing object " << name;
                    gameState.getGameWorld().removeObject(name);
                }
                sprite->getPosition()->setX(position.first);
                sprite->getPosition()->setY(position.second);
            }
        }

        // local input
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // handle game input
            else if (e.type == SDL_KEYDOWN) {
                auto pair = gameInput.handleInputFPS(keystate, gameState, targetFPS);
                quit = pair.first;
                localFPS = pair.second;
                timePerFrame = 1000 / localFPS;
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

        // Create threads for movement and input
        std::thread inputThread(handleInput, std::ref(gameState), keystate);
        std::thread movementThread(handleMovement, std::ref(gameState));
        inputThread.join();
        movementThread.join();

        // Render the gamestate
        windowManager.updateWindow(gameState);

        // Send client updates to server
        NamedSprite* player = gameWorld.getObjectByName(client.getPlayerName());
        movingSprites = client.sendInput(player->getName(), { player->getPosition()->getX(), player->getPosition()->getY()});

        std::set<std::string> localSpriteNames;
        for (auto& sprite : gameWorld.getObjects()) {
            localSpriteNames.insert(sprite.first);
        }

        std::set<std::string> serverSpriteNames;
        for (auto& sprite : movingSprites) {
            serverSpriteNames.insert(sprite.first);
        }

        for (const auto& name : localSpriteNames) {
            if (serverSpriteNames.find(name) == serverSpriteNames.end() && (name.find("platform") != std::string::npos || name.find("Player") != std::string::npos)) {
                gameWorld.removeObject(name);
            }
        }

        // Frame-rate limiting
        int64_t frameEndTime = globalTimeline.getTime();
        int64_t frameDuration = frameEndTime - frameStartTime;
        int64_t sleepTime = timePerFrame - frameDuration;
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }
    NamedSprite* player = gameWorld.getObjectByName(client.getPlayerName());
    client.deleteClient(player->getName(), {-1, -1});
    windowManager.close();
    return 0;
}
