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
#include "GameTypes.h"
#include "EventManager.h"
#include "InputEventHandler.h"
#include "CollisionEventHandler.h"
#include "DeathEventHandler.h"
#include "SpawnEventHandler.h"
#include "TextureComponent.h"
#include <SDL.h>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <set>
#include <tuple>

// Synchronization primitives
std::mutex mtx;

// Thread function to handle input
void handleInput(GameState& gameState, const Uint8* keystate) {
    std::unique_lock<std::mutex> lock(mtx);
    gameState.stepInput(keystate);
    gameState.processPendingEvents();
    gameState.getEventManager().update();
}

// Thread function to handle movement
void handleMovement(GameState& gameState) {
    std::unique_lock<std::mutex> lock(mtx);
    gameState.stepMovement();
    gameState.processPendingEvents();
    gameState.getEventManager().update();
}

int main(int argc, char* args[]) {
    // Initialize the client (server IP, port REQ-REP)
    JClient client("127.0.0.1", 5555);

    // Ask the server for the player number and add client (e.g., PlayerX)
    client.connectToServer();

    // Initialize game engine components
    Timeline globalTimeline(nullptr, 1);
    PhysicsEngine physics;
    WindowManager windowManager;
    GameWorld gameWorld(800, 600);
    GameTypes types;
    GameState gameState(globalTimeline, gameWorld, physics, windowManager, types, client);
    GameInput gameInput;

    auto& eventManager = gameState.getEventManager(); // Get reference to event manager
    eventManager.registerHandler("Input", std::make_shared<InputEventHandler>(gameState));
    eventManager.registerHandler("Collision", std::make_shared<CollisionEventHandler>());
    eventManager.registerHandler("Death", std::make_shared<DeathEventHandler>(gameState));
    eventManager.registerHandler("Spawn", std::make_shared<SpawnEventHandler>());

    SDL_Renderer* renderer = nullptr;

    // Initialize WindowManager and renderer
    if (!windowManager.init(800, 600)) {
        std::cerr << "Failed to initialize WindowManager!" << std::endl;
        return -1;
    }
    else {
        renderer = gameState.getWindowManager().getRenderer();
        if (!renderer) {
            std::cerr << "Renderer is null in getRenderer!" << std::endl;
            return -1;
        }
    }


    // Configure the physics engine
    gameState.getPhysicsEngine().setPixelsPerMeter(16.0);
    gameState.getPhysicsEngine().setGravity(5.0 / 100.0);

    // colors
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Color cyan = { 0, 255, 255, 0 };
    SDL_Color black = { 0, 0, 0, 255 };
    double spawnX = 20.0;
    double spawnY = 400.0;

    // define game types (expected types are "player" and "platform", can customize further if needed)
    size_t player_type = types.getTypeHash("player");
    size_t platform_type = types.getTypeHash("platform");
    size_t fixed_type = types.getTypeHash("fixed");
    size_t obstacle_type = types.getTypeHash("obstacle");

    // Create the fixed shape (must match server's)
    PositionComponent* fixedPos = new StandardPosition(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
    DrawerComponent* fixedDrawer = new SimpleRectDrawer(green);
    Timeline fixedTimeline(&globalTimeline, 64);
    NamedSprite* fixedShape = new NamedSprite("ground", fixed_type, fixedPos, nullptr, nullptr, fixedDrawer, fixedTimeline, spawnX, spawnY);
    gameWorld.addObject(fixedShape);

    PositionComponent* splatformPos = new StandardPosition(150.0, 400.0, 100, 20);
    DrawerComponent* splatformDrawer = new SimpleRectDrawer(cyan);
    Timeline splatformTimeline(&globalTimeline, 64);
    NamedSprite* splatform = new NamedSprite("splatform", fixed_type, splatformPos, nullptr, nullptr, splatformDrawer, splatformTimeline, spawnX, spawnY);
    gameWorld.addObject(splatform);

    PositionComponent* splatform1Pos = new StandardPosition(550.0, 400.0, 100, 20);
    DrawerComponent* splatform1Drawer = new SimpleRectDrawer(cyan);
    Timeline splatform1Timeline(&globalTimeline, 64);
    NamedSprite* splatform1 = new NamedSprite("splatform1", fixed_type, splatform1Pos, nullptr, nullptr, splatform1Drawer, splatform1Timeline, spawnX, spawnY);
    gameWorld.addObject(splatform1);

    // Create the fixed obstacle shape (must match server's)

    PositionComponent* fixedPos3 = new StandardPosition(785.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 15, 100);
    DrawerComponent* fixedDrawer3 = new SimpleRectDrawer(black);
    Timeline fixedTimeline3(&globalTimeline, 64);
    NamedSprite* fixedShape3 = new NamedSprite("obstacle3", obstacle_type, fixedPos3, nullptr, nullptr, fixedDrawer3, fixedTimeline3, spawnX, spawnY);
    gameWorld.addObject(fixedShape3);

    // Create the ControllableJumper(player object) for the client
    std::string name = client.getPlayerName();
    gameInput.setPlayerName(name);
    int playerNum = std::stoi(name.substr(6));
    PositionComponent* jumperPos = new StandardPosition(50.0, 0.0 + playerNum * 50, 50, 50);
    ControllableJumper* jumperMovement = new ControllableJumper(45.0 / 100.0, 0.0, 2.5);  // Handles movement and input
    DrawerComponent* jumperDrawer = new SimpleRectDrawer(red);
    Timeline jumperTimeline(&globalTimeline, 10);
    NamedSprite* jumper = new NamedSprite(name, player_type, jumperPos, jumperMovement, jumperMovement, jumperDrawer, jumperTimeline, spawnX, spawnY);
    // Add a texture to the jumper
    TextureComponent* jumperTexture = new TextureComponent(renderer, "spaceship.bmp");
    if (jumperTexture->isLoaded()) {
        jumper->setTexture(jumperTexture);
        std::cout << "Texture loaded successfully for jumper!" << std::endl;
    }
    else {
        std::cerr << "Failed to load texture for jumper!" << std::endl;
    }

    gameWorld.addObject(jumper);


    // Main client loop
    bool quit = false;
    SDL_Event e;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    int targetFPS = 30;  // Frame rate target
    int localFPS = targetFPS;
    int64_t timePerFrame = 1000 / targetFPS;  // Milliseconds per frame
    std::unordered_map<std::string, std::tuple<size_t, double, double>> movingSprites;

    while (!quit) {
        // Start frame timing
        int64_t frameStartTime = globalTimeline.getTime();

        // If there are new shapes in the server's game state, add them as fixed shapes
        for (const auto& spriteData : movingSprites) {
            const std::string& name = spriteData.first;
            const std::tuple<size_t, double, double>& data = spriteData.second;  // Tuple of (type, x, y)
            size_t type = std::get<0>(data);
            double x = std::get<1>(data);
            double y = std::get<2>(data);

            NamedSprite* sprite = gameWorld.getObjectByName(name);
            if (!sprite) {
                // Determine the drawer color based on the name
                // SDL_Color drawerColor = (name.find("Player") != std::string::npos) ? red : green;
                SDL_Color drawerColor = (type == player_type) ? red : green;

                if (x == -1.0) {
                    continue;
                }

                // If the object doesn't exist, create it as a fixed shape
                PositionComponent* fixedPos = new StandardPosition(x, y, 50, 50);
                DrawerComponent* fixedDrawer = new SimpleRectDrawer(drawerColor);
                NamedSprite* newSprite = new NamedSprite(name, player_type, fixedPos, nullptr, nullptr, fixedDrawer, globalTimeline, spawnX, spawnY);
                gameWorld.addObject(newSprite);
                std::cout << "Added " << name << " as a fixed shape to the game world." << std::endl;
            }
            else {
                // Update the position of existing moving shapes
                if (x == -1.0) {
                    std::cout << "removing object " << name;
                    gameState.getGameWorld().removeObject(name);
                }
                sprite->getPosition()->setX(x);
                sprite->getPosition()->setY(y);
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
        movingSprites = client.sendInput(player->getName(), player_type, { player->getPosition()->getX(), player->getPosition()->getY() }, false);

        std::set<std::string> localSpriteNames;
        for (auto& sprite : gameWorld.getObjects()) {
            localSpriteNames.insert(sprite.first);
        }

        std::set<std::string> serverSpriteNames;
        for (auto& sprite : movingSprites) {
            serverSpriteNames.insert(sprite.first);
        }

        for (const auto& name : localSpriteNames) {
            if (serverSpriteNames.find(name) == serverSpriteNames.end() && (name.find("Player") != std::string::npos)) {
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
    //client.deleteClient(player->getName(), {-1, -1});
    client.deleteClient(player->getTypeHash());
    windowManager.close();
    return 0;
}
