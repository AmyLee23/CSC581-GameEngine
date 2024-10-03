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
#include <thread>
#include <mutex>
#include <iostream>

// Synchronization primitives
std::mutex mtx;

// Thread function to handle movement
void handleMovement(GameState& gameState) {
    std::unique_lock<std::mutex> lock(mtx);
    gameState.stepMovement();  // Call stepMovement to update the game state
}

int main(int argc, char* args[]) {
    // Initialize the server (ports for PUSH-PULL and PUB-SUB, and REQ-REP for player number)
    JServer server(5555); 

    // Initialize game engine components
    Timeline globalTimeline(nullptr, 1);
    PhysicsEngine physics;
    WindowManager windowManager;  // Not used for rendering in server, just in GameState
    GameWorld gameWorld(800, 600);
    GameState gameState(globalTimeline, gameWorld, physics, windowManager);

    // Configure the physics engine
    gameState.getPhysicsEngine().setPixelsPerMeter(16.0);
    gameState.getPhysicsEngine().setGravity(5.0 / 100.0);

    // colors
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color red = { 255, 0, 0, 255 };

    // Fixed shape (ground)
    PositionComponent* groundPos = new StandardPosition(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
    DrawerComponent* groundDrawer = new SimpleRectDrawer(green);
    Timeline groundTimeline(&globalTimeline, 64);
    NamedSprite* ground = new NamedSprite("ground", groundPos, nullptr, nullptr, groundDrawer, groundTimeline);
    gameWorld.addObject(ground);

    // Bouncing shape (platform)
    PositionComponent* platformPos = new StandardPosition(500.0, 200.0, 50, 50);
    MovementComponent* platformMovement = new BouncingMovement(30.0 / 100.0, 0);  // Speed x, no vertical movement
    DrawerComponent* platformDrawer = new SimpleRectDrawer(green);
    Timeline platformTimeline(&globalTimeline, 10);
    NamedSprite* platform = new NamedSprite("platform", platformPos, platformMovement, nullptr, platformDrawer, platformTimeline);
    gameWorld.addObject(platform);

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
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
                quit = true;
            }
        }

        // Handle incoming REQ-REP requests for player numbers
        server.handlePlayerNumberRequests(gameState);

        // Check for new clients and update last_num_clients
        int current_num_clients = server.getNumClients();
        if (current_num_clients > last_num_clients) {
            // Add new clients to the game world as fixed shapes
            for (int i = last_num_clients + 1; i <= current_num_clients; ++i) {
                std::string clientName = "Player" + std::to_string(i);
                PositionComponent* clientPos = new StandardPosition(50.0, 0.0 + i * 50, 50, 50);
                DrawerComponent* clientDrawer = new SimpleRectDrawer(red);
                NamedSprite* clientSprite = new NamedSprite(clientName, clientPos, nullptr, nullptr, clientDrawer, globalTimeline);
                gameWorld.addObject(clientSprite);
                std::cout << "Added " << clientName << " to the game world." << std::endl;
            }
            last_num_clients = current_num_clients;  // Update the count
        }

        // thread to handle movement
        std::thread movementThread(handleMovement, std::ref(gameState));
        movementThread.join();

        /*
        // Get all moving object positions
        std::unordered_map<std::string, std::pair<double, double>> movingSprites;
        for (const auto& object_pair : gameWorld.getObjects()) {
            NamedSprite* object = object_pair.second;
            const std::string& name = object_pair.first;
            if ((name.find("platform") != std::string::npos || name.find("Player") != std::string::npos)) {
                movingSprites[name] = { object->getPosition()->getX(), object->getPosition()->getY() };
            }
        }

        // Broadcast the game state to all clients
        server.broadcastGameState(movingSprites); */

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
