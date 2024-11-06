#include "pch.h"
#include "ControllableJumper.h"
#include "NamedSprite.h"
#include "PhysicsEngine.h"
#include "GameWorld.h"
#include "CollisionEvent.h"
#include <iostream>

ControllableJumper::ControllableJumper(double dx, double dy, double jumpVelocity)
    : dx(0), speed(dx), dy(dy), isJumping(false), isOnGround(false), jumpVelocity(jumpVelocity) {}

double ControllableJumper::getDx() const { return dx; }
double ControllableJumper::getDy() const { return dy; }
double ControllableJumper::getJumpVelocity() const { return jumpVelocity; }
bool ControllableJumper::getIsJumping() const { return isJumping; }
bool ControllableJumper::getIsOnGround() const { return isOnGround; }

void ControllableJumper::setDx(double dx) { this->dx = dx; }
void ControllableJumper::setDy(double dy) { this->dy = dy; }
void ControllableJumper::setJumpVelocity(double jumpVelocity) { this->jumpVelocity = jumpVelocity; }
void ControllableJumper::setIsJumping(bool isJumping) { this->isJumping = isJumping; }
void ControllableJumper::setIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; }

void ControllableJumper::move(NamedSprite& sprite, GameState& gameState, double dt) {
    std::lock_guard<std::mutex> lock(mtx);
    PositionComponent& position = *sprite.getPosition();
    PhysicsEngine& physics = gameState.getPhysicsEngine();
    GameWorld& gameWorld = gameState.getGameWorld();

    double lastY = position.getY();

    // apply gravity (modify dy only)
    physics.applyGravityDy(sprite, dt);

    double moveY = dy * dt * physics.getPixelsPerMeter();
    double maxMove = physics.getMaxPixelMovement();

    // Calculate the number of steps needed to break down the movement
    int stepsY = static_cast<int>(std::ceil(std::abs(moveY) / maxMove));
    double incrementY = moveY / stepsY;

    for (int step = 0; step < stepsY; ++step) {
        position.setY(lastY - incrementY);

        // Check for collisions
        if (physics.checkCollisions(sprite, gameState)) {
            if (physics.checkCollisions(sprite, gameState)) {
                dy = 0;
                gameState.getEventManager().raiseEvent(
                    std::make_shared<CollisionEvent>(
                        sprite.getTimeline(),
                        sprite,
                        position.getX(),
                        lastY,
                        lastY < position.getY(),
                        isOnGround,
                        false 
                    )
                );

                if (isOnGround) {
                    position.setY(lastY);
                    break;
                }

                position.setY(lastY);
            }
            else {
                lastY = position.getY();
            }
        }
    }

    ////====== Handling Horizontal Movement ======
    if (dx != 0) {
        double lastX = position.getX();
        double moveX = dx * dt * physics.getPixelsPerMeter();
        double maxMove = physics.getMaxPixelMovement();

        int stepsX = static_cast<int>(std::ceil(std::abs(moveX) / maxMove));
        double incrementX = moveX / stepsX;

        for (int step = 0; step < stepsX; ++step) {
            position.setX(lastX + incrementX);

            // Check for collisions after each incremental movement
            if (position.getX() < 0 ||
                position.getX() + position.getWidth() > gameWorld.getWorldWidth() ||
                physics.checkCollisions(sprite, gameState)) {

                gameState.getEventManager().raiseEvent(
                    std::make_shared<CollisionEvent>(
                        sprite.getTimeline(),
                        sprite,
                        lastX,
                        position.getY(),
                        lastY < position.getY(),
                        isOnGround,
                        true
                    )
                );

                position.setX(lastX);  // Reset to last valid position
                break;  // Stop movement after collision
            }
            else {
                lastX = position.getX();  // Update last valid position
            }
        }
    }
}

void ControllableJumper::handleInput(const Uint8* keystate, NamedSprite& sprite, GameState& gameState, double dt) {
    std::lock_guard<std::mutex> lock(mtx);
    PositionComponent& position = *sprite.getPosition();
    GameWorld& gameWorld = gameState.getGameWorld();
    PhysicsEngine& physics = gameState.getPhysicsEngine();

    double lastX = position.getX();
    double lastY = position.getY();

    double metersMovedX = dx * dt;
    double metersMovedY = dy * dt;
    double moveX = metersMovedX * physics.getPixelsPerMeter();
    double moveY = metersMovedY * physics.getPixelsPerMeter();
    double maxMove = physics.getMaxPixelMovement();

    // calculate the number of steps needed
    int stepsX = static_cast<int>(std::ceil(std::abs(moveX) / maxMove));
    int stepsY = static_cast<int>(std::ceil(std::abs(moveY) / maxMove));
    int totalSteps = std::max(stepsX, stepsY);

    double incrementX = moveX / totalSteps;
    double incrementY = moveY / totalSteps;

    // handle jumping
    if (keystate[SDL_SCANCODE_SPACE] && isOnGround) {
        dy = jumpVelocity;  // start jumping by setting the vertical velocity
        isJumping = true;
        isOnGround = false;
    }

    if (keystate[SDL_SCANCODE_LEFT]) {
        dx = -speed;
        //position.setX(lastX - incrementX);
    }else if (keystate[SDL_SCANCODE_RIGHT]) {
        dx = speed;
        //position.setX(lastX + incrementX);
    }
    else {
        dx = 0;
    }
    

    // Incrementally handle movement
    //for (int step = 0; step < totalSteps; ++step) {
    //    // increment horizontal movement
    //    if (keystate[SDL_SCANCODE_LEFT]) {
    //        position.setX(lastX - incrementX);
    //    }
    //    if (keystate[SDL_SCANCODE_RIGHT]) {
    //        position.setX(lastX + incrementX);
    //    }
    //    // check horizontal collisions
    //    if (position.getX() < 0 || position.getX() + position.getWidth() > gameWorld.getWorldWidth() || physics.checkCollisions(sprite, gameState)) {
    //        //position.setX(lastX);  // reset to last valid position
    //        //break;  // stop horizontal movement on collision
    //        gameState.getEventManager().raiseEvent(
    //            std::make_shared<CollisionEvent>(
    //                sprite.getTimeline(),
    //                sprite,
    //                lastX,           // lastValidX
    //                position.getY(), // current Y
    //                false,          // not moving down
    //                isOnGround,     // reference to isOnGround
    //                true           // is horizontal collision
    //            )
    //        );
    //        break;
    //    }
    //    else {
    //        lastX = position.getX();  // Update lastX to the new valid position
    //    }
    //}
}