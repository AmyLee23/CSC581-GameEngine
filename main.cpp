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

#include <thread>

double adjustTimeScale(double currentScale, double adjustment, double minScale, double maxScale) {
    double newScale = currentScale + adjustment;
    if (newScale > maxScale) {
        return maxScale;
    }
    if (newScale < minScale) {
        return minScale;
    }
    return newScale;
}

int main(int argc, char* args[]) {
    WindowManager windowManager;
    PhysicsEngine physics;
    Timeline globalTimeline(nullptr, 1);
    GameWorld gameWorld(800, 600);
    GameState gameState(globalTimeline, gameWorld, physics, windowManager);

    gameState.getPhysicsEngine().setPixelsPerMeter(16.0);

    if (!windowManager.init(900, 700)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    SDL_Color green = { 0, 255, 0, 255 };
    // fixed shape
    PositionComponent* fp = new StandardPosition(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
    DrawerComponent* fd = new SimpleRectDrawer(green);
    Timeline localTimeline1(&globalTimeline, 64);
    NamedSprite* ground = new NamedSprite("ground", fp, nullptr, nullptr, fd, localTimeline1);
    gameWorld.addObject(ground);

    // bouncing shape
    PositionComponent* bp = new StandardPosition(500.0, 200.0, 80, 60);
    MovementComponent* bm = new BouncingMovement(30.0 / 100.0, 0);
    DrawerComponent* bd = new SimpleRectDrawer(green);
    Timeline localTimeline2(&globalTimeline, 10);
    NamedSprite* platform = new NamedSprite("platform", bp, bm, nullptr, bd, localTimeline2);
    gameWorld.addObject(platform);

    SDL_Color red = { 255, 0, 0, 255 };
    // controllable flyer
    //SDL_Color red = { 255, 0, 0, 255 };
    //PositionComponent* pp = new StandardPosition(100.0, 100.0, 50, 50);
    //InputComponent* pi = new ControllableFlyer(60.0 / 100.0, 60.0 / 100.0);
    //DrawerComponent* pd = new SimpleRectDrawer(red);
    //Timeline localTimeline3(&globalTimeline, 10);
    //NamedSprite* flyer = new NamedSprite("flyer", pp, nullptr, pi, pd, localTimeline3);
    //gameWorld.addObject(flyer);

    // controllable jumper
    gameState.getPhysicsEngine().setGravity(5.0 / 100.0);
    PositionComponent* jp = new StandardPosition(100.0, 100.0, 50, 50);
    ControllableJumper* jmi = new ControllableJumper(45.0 / 100.0, 0.0, 1.5);  // Handles both movement and input
    DrawerComponent* jd = new SimpleRectDrawer(red);
    Timeline localTimeline4(&globalTimeline, 10);
    NamedSprite* jumper = new NamedSprite("jumper", jp, jmi, jmi, jd, localTimeline4);
    gameWorld.addObject(jumper);

    bool quit = false;
    SDL_Event e;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    int targetFPS = 120;
    int64_t timePerFrame = 1000 / targetFPS;

    while (!quit) {
        int64_t frameStartTime = globalTimeline.getTime();

        while (SDL_PollEvent(&e) != 0) {
            // q to quit
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
                quit = true;
            }
            // window resize
            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int windowWidth = e.window.data1;
                    int windowHeight = e.window.data2;
                    windowManager.updateWindowSize(windowWidth, windowHeight, gameState);
                }
            }
            // s to scale shapes, p to pause/unpause
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_s) {
                    gameState.getWindowManager().toggleProportionalMode();
                }
                else if (e.key.keysym.sym == SDLK_p) {
                    if (globalTimeline.isPaused()) {
                        globalTimeline.unpause();
                    }
                    else {
                        globalTimeline.pause();
                    }
                }
            }
            // - to slow down, + to speed up
            if (e.type == SDL_KEYDOWN) {
                double adjustment = 0.0;
                if (e.key.keysym.sym == SDLK_EQUALS || e.key.keysym.sym == SDLK_PLUS) {
                    adjustment = 0.1;
                }
                else if (e.key.keysym.sym == SDLK_MINUS) {
                    adjustment = -0.1;
                }
                if (adjustment != 0.0) {
                    double newGlobalTimeScale = adjustTimeScale(globalTimeline.getTimeScale(), adjustment, 0.5, 2.0);
                    globalTimeline.setTimeScale(newGlobalTimeScale);
                    for (auto& pair : gameWorld.getObjects()) {
                        NamedSprite* sprite = pair.second;
                        Timeline& spriteTimeline = sprite->getTimeline();
                        double newSpriteTimeScale = adjustTimeScale(spriteTimeline.getTimeScale(), adjustment, 0.5, 2.0);
                        spriteTimeline.setTimeScale(newSpriteTimeScale);
                    }
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

//// fixed shape
//auto fp = std::make_unique<StandardPosition>(0.0, static_cast<double>(gameWorld.getWorldHeight() - 50), gameWorld.getWorldWidth(), 50);
//SDL_Color green = { 0, 255, 0, 255 };
//auto fd = std::make_unique<SimpleRectDrawer>(green);
//Timeline localTimeline1(&globalTimeline, 64);
//auto ground = std::make_unique<NamedSprite>("ground", std::move(fp), nullptr, nullptr, std::move(fd), localTimeline1);
//gameWorld.addObject(ground.get());
//
//// bouncing shape
//auto bp = std::make_unique<StandardPosition>(500.0, 200.0, 80, 60);
//auto bm = std::make_unique<BouncingMovement>(30.0/100.0, 0);
//auto bd = std::make_unique<SimpleRectDrawer>(green);
//Timeline localTimeline2(&globalTimeline, 10);
//auto platform = std::make_unique<NamedSprite>("platform", std::move(bp), std::move(bm), nullptr, std::move(bd), localTimeline2);
//gameWorld.addObject(platform.get());
//
//SDL_Color red = { 255, 0, 0, 255 };
//// controllable flyer
//auto pp = std::make_unique<StandardPosition>(100.0, 100.0, 50, 50);
//auto pi = std::make_unique<ControllableFlyer>(60.0 / 100.0, 60.0/100.0);
//auto pd = std::make_unique<SimpleRectDrawer>(red);
//Timeline localTimeline3(&globalTimeline, 10);
//auto flyer = std::make_unique<NamedSprite>("flyer", std::move(pp), nullptr, std::move(pi), std::move(pd), localTimeline3);
//gameWorld.addObject(flyer.get());
//
//// controllable jumper
//gameState.getPhysicsEngine().setGravity(30.0 / 100.0);
//auto jp = std::make_unique<StandardPosition>(100.0, 100.0, 50, 50);
//auto jmi = std::make_unique<ControllableJumper>(45.0 / 100.0, 0.0, 45.0 / 100.0);
//auto jd = std::make_unique<SimpleRectDrawer>(red);
//Timeline localTimeline4(&globalTimeline, 10);
//auto jumper = std::make_unique<NamedSprite>("jumper", std::move(jp), jmi.get(), jmi.get(), std::move(jd), localTimeline4);
//gameWorld.addObject(jumper.get());