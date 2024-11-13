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
    GameWorld gameWorld(1600, 600);
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
    double spawnX = 20.0;
    double spawnY = 200.0;

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

    PositionComponent* groundPos1 = new StandardPosition(200.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 10, 100);
    DrawerComponent* groundDrawer1 = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline groundTimeline1(&globalTimeline, 64);
    NamedSprite* obstacle1 = new NamedSprite("obstacle1", fixed_type, groundPos1, nullptr, nullptr, groundDrawer1, groundTimeline1, spawnX, spawnY);
    gameWorld.addObject(obstacle1);

    PositionComponent* groundPos2 = new StandardPosition(190.0, static_cast<double>(gameWorld.getWorldHeight() - 170), 10, 40);
    DrawerComponent* groundDrawer2 = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline groundTimeline2(&globalTimeline, 64);
    NamedSprite* obstacle1_1 = new NamedSprite("obstacle1_1", obstacle_type, groundPos2, nullptr, nullptr, groundDrawer2, groundTimeline2, spawnX, spawnY);
    gameWorld.addObject(obstacle1_1);

    PositionComponent* groundPos3 = new StandardPosition(210.0, static_cast<double>(gameWorld.getWorldHeight() - 170), 10, 40);
    DrawerComponent* groundDrawer3 = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline groundTimeline3(&globalTimeline, 64);
    NamedSprite* obstacle1_2 = new NamedSprite("obstacle1_2", obstacle_type, groundPos3, nullptr, nullptr, groundDrawer3, groundTimeline3, spawnX, spawnY);
    gameWorld.addObject(obstacle1_2);

    PositionComponent* groundPos4 = new StandardPosition(600.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 10, 100);
    DrawerComponent* groundDrawer4 = new SimpleRectDrawer({ 92, 28, 0, 255 });
    Timeline groundTimeline4(&globalTimeline, 64);
    NamedSprite* obstacle2 = new NamedSprite("obstacle2", fixed_type, groundPos4, nullptr, nullptr, groundDrawer4, groundTimeline4, spawnX, spawnY);
    gameWorld.addObject(obstacle2);

    PositionComponent* groundPos5 = new StandardPosition(590.0, static_cast<double>(gameWorld.getWorldHeight() - 170), 10, 40);
    DrawerComponent* groundDrawer5 = new SimpleRectDrawer({ 92, 28, 0, 255 });
    Timeline groundTimeline5(&globalTimeline, 64);
    NamedSprite* obstacle2_1 = new NamedSprite("obstacle2_1", obstacle_type, groundPos5, nullptr, nullptr, groundDrawer5, groundTimeline5, spawnX, spawnY);
    gameWorld.addObject(obstacle2_1);

    PositionComponent* groundPos6 = new StandardPosition(610.0, static_cast<double>(gameWorld.getWorldHeight() - 170), 10, 40);
    DrawerComponent* groundDrawer6 = new SimpleRectDrawer({ 92, 28, 0, 255 });
    Timeline groundTimeline6(&globalTimeline, 64);
    NamedSprite* obstacle2_2 = new NamedSprite("obstacle2_2", obstacle_type, groundPos6, nullptr, nullptr, groundDrawer6, groundTimeline6, spawnX, spawnY);
    gameWorld.addObject(obstacle2_2);

    PositionComponent* groundPos7 = new StandardPosition(1000.0, static_cast<double>(gameWorld.getWorldHeight() - 150), 10, 100);
    DrawerComponent* groundDrawer7 = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline groundTimeline7(&globalTimeline, 64);
    NamedSprite* obstacle3 = new NamedSprite("obstacle3", fixed_type, groundPos7, nullptr, nullptr, groundDrawer7, groundTimeline7, spawnX, spawnY);
    gameWorld.addObject(obstacle3);


    PositionComponent* groundPos8 = new StandardPosition(990.0, static_cast<double>(gameWorld.getWorldHeight() - 170), 10, 40);
    DrawerComponent* groundDrawer8 = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline groundTimeline8(&globalTimeline, 64);
    NamedSprite* obstacle3_1 = new NamedSprite("obstacle3_1", obstacle_type, groundPos8, nullptr, nullptr, groundDrawer8, groundTimeline8, spawnX, spawnY);
    gameWorld.addObject(obstacle3_1);

    PositionComponent* groundPos9 = new StandardPosition(1010.0, static_cast<double>(gameWorld.getWorldHeight() - 170), 10, 40);
    DrawerComponent* groundDrawer9 = new SimpleRectDrawer({ 150, 75, 0, 255 });
    Timeline groundTimeline9(&globalTimeline, 64);
    NamedSprite* obstacle3_2 = new NamedSprite("obstacle3_2", obstacle_type, groundPos9, nullptr, nullptr, groundDrawer9, groundTimeline9, spawnX, spawnY);
    gameWorld.addObject(obstacle3_2);

    // Bouncing shape (platform)
    PositionComponent* platformPos = new StandardPosition(500.0, 200.0, 50, 50);
    MovementComponent* platformMovement = new BouncingMovement(30.0 / 100.0, 0);
    DrawerComponent* platformDrawer = new SimpleRectDrawer(green);
    Timeline platformTimeline(&globalTimeline, 10);
    NamedSprite* moving_platform = new NamedSprite("platform", platform_type, platformPos, platformMovement, nullptr, platformDrawer, platformTimeline, spawnX, spawnY);
    gameWorld.addObject(moving_platform);

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