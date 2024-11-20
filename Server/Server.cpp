#include "JServer.h"
#include "GameWorld.h"
#include "GameState.h"
#include "Timeline.h"
#include "PhysicsEngine.h"
#include "WindowManager.h"
#include "NamedSprite.h"
#include "StandardPosition.h"
#include "BouncingMovement.h"
#include "SimpleRectDrawer.h"
#include "GameTypes.h"
#include "DeathEventHandler.h"
#include "SpawnEventHandler.h"
#include <thread>
#include <mutex>
#include <iostream>

// Synchronization primitives
std::mutex mtx;

// Thread function to handle movement
void handleMovement(GameState& gameState) {
    std::unique_lock<std::mutex> lock(mtx);
    gameState.stepMovement();
    gameState.processPendingEvents();
    gameState.getEventManager().update();
}

int main(int argc, char* args[]) {
    // Initialize the server
    JServer server(5555);
    // Initialize game engine components
    Timeline globalTimeline(nullptr, 1);
    PhysicsEngine physics;
    WindowManager windowManager;
    GameWorld gameWorld(800, 600);
    GameTypes types;
    GameState gameState(globalTimeline, gameWorld, physics, windowManager, types);

    auto& eventManager = gameState.getEventManager(); // Get reference to event manager
    eventManager.registerHandler("Death", std::make_shared<DeathEventHandler>(gameState));
    eventManager.registerHandler("Spawn", std::make_shared<SpawnEventHandler>());

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

    // Fixed shape (platform)
    PositionComponent* groundPos = new StandardPosition(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
    DrawerComponent* groundDrawer = new SimpleRectDrawer(green);
    Timeline groundTimeline(&globalTimeline, 64);
    NamedSprite* ground = new NamedSprite("ground", fixed_type, groundPos, nullptr, nullptr, groundDrawer, groundTimeline, spawnX, spawnY);
    gameWorld.addObject(ground);

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

    PositionComponent* fixedPos3 = new StandardPosition(785.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 15, 100);
    DrawerComponent* fixedDrawer3 = new SimpleRectDrawer(black);
    Timeline fixedTimeline3(&globalTimeline, 64);
    NamedSprite* fixedShape3 = new NamedSprite("obstacle3", obstacle_type, fixedPos3, nullptr, nullptr, fixedDrawer3, fixedTimeline3, spawnX, spawnY);
    gameWorld.addObject(fixedShape3);

    // Bouncing shape (platform)
    PositionComponent* platformPos = new StandardPosition(500.0, 200.0, 50, 50);
    MovementComponent* platformMovement = new BouncingMovement(30.0 / 100.0, 0);
    DrawerComponent* platformDrawer = new SimpleRectDrawer(green);
    Timeline platformTimeline(&globalTimeline, 10);
    NamedSprite* moving_platform = new NamedSprite("platform", platform_type, platformPos, platformMovement, nullptr, platformDrawer, platformTimeline, spawnX, spawnY);
    gameWorld.addObject(moving_platform);

    PositionComponent* platform1Pos = new StandardPosition(400.0, 100.0, 50, 50);
    MovementComponent* platform1Movement = new BouncingMovement(0.0, 30.0 / 100.0);
    DrawerComponent* platform1Drawer = new SimpleRectDrawer(green);
    Timeline platform1Timeline(&globalTimeline, 10);
    NamedSprite* moving_platform1 = new NamedSprite("platform1", platform_type, platform1Pos, platform1Movement, nullptr, platform1Drawer, platform1Timeline, spawnX, spawnY);
    gameWorld.addObject(moving_platform1);
    // Track the number of clients
    int last_num_clients = 0;  // Initial client count

    // Main server loop
    SDL_Event e;
    bool quit = false;
    int targetFPS = 120;  // Frame rate target
    int64_t timePerFrame = 1000 / targetFPS;  // Milliseconds per frame

    while (!quit) {
        int64_t frameStartTime = globalTimeline.getTime();

        // quit server with 'q'
        // TODO: not working, probably because is not an SDL window
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
                quit = true;
            }
        }

        // Handle incoming REQ-REP requests to connect
        server.handleConnectionRequests(gameState);

        // Check for new clients and update last_num_clients
        int current_num_clients = server.getNumClients();
        if (current_num_clients > last_num_clients) {
            // Add new clients to the game world as fixed shapes
            for (int i = last_num_clients + 1; i <= current_num_clients; ++i) {
                std::string clientName = "Player" + std::to_string(i);
                PositionComponent* clientPos = new StandardPosition(50.0, 0.0 + i * 50, 50, 50);
                DrawerComponent* clientDrawer = new SimpleRectDrawer(red);
                NamedSprite* clientSprite = new NamedSprite(clientName, player_type, clientPos, nullptr, nullptr, clientDrawer, globalTimeline, spawnX, spawnY);
                gameWorld.addObject(clientSprite);
                std::cout << "Added " << clientName << " to the game world." << std::endl;
            }
            last_num_clients = current_num_clients;  // Update the count
        }

        // thread to handle movement
        std::thread movementThread(handleMovement, std::ref(gameState));
        movementThread.join();

        // Frame-rate limiting
        int64_t frameEndTime = globalTimeline.getTime();
        int64_t frameDuration = frameEndTime - frameStartTime;
        int64_t sleepTime = timePerFrame - frameDuration;
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }

    return 0;
}