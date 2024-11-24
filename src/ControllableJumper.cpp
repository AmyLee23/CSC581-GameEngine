#include "pch.h"
#include "ControllableJumper.h"
#include "NamedSprite.h"
#include "PhysicsEngine.h"
#include "GameWorld.h"
#include "CollisionEvent.h"
#include "DeathEvent.h"
#include "GameTypes.h"
#include "StartRecordingEvent.h"
#include "StopRecordingEvent.h"
#include "PlayReplayEvent.h"
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
    GameTypes& types = gameState.getGameTypes();

    //====== Handling Vertical Movement ======
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

        if (physics.checkCollisionsWithType(types.getTypeHash("obstacle"), sprite, gameState)) {
            gameState.queueEvent(
                std::make_shared<DeathEvent>(
                    sprite.getTimeline(),
                    sprite
                )
            );
            return;
        }

        // Check for collisions
        if (physics.checkCollisions(sprite, gameState)) {
            if (physics.checkCollisions(sprite, gameState)) {
                dy = 0;
                gameState.queueEvent(
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

    //====== Handling Horizontal Movement ======
    if (dx != 0) {
        double lastX = position.getX();
        double moveX = dx * dt * physics.getPixelsPerMeter();
        double maxMove = physics.getMaxPixelMovement();

        int stepsX = static_cast<int>(std::ceil(std::abs(moveX) / maxMove));
        double incrementX = moveX / stepsX;

        for (int step = 0; step < stepsX; ++step) {
            position.setX(lastX + incrementX);

            if (physics.checkCollisionsWithType(types.getTypeHash("obstacle"), sprite, gameState) and !isDashing) {
                gameState.queueEvent(
                    std::make_shared<DeathEvent>(
                        sprite.getTimeline(),
                        sprite
                    )
                );
                return;
            }

            // Check for collisions after each incremental movement
            if (position.getX() < 0 ||
                position.getX() + position.getWidth() > gameWorld.getWorldWidth() ||
                (physics.checkCollisions(sprite, gameState) and !isDashing)) {
                gameState.queueEvent(
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

void ControllableJumper::startDash(int direction) {
    isDashing = true;
    dashStartTime = std::chrono::steady_clock::now();

    // Set dx to a higher speed in the given direction
    dx = direction * speed * dashSpeedMultiplier;
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

    // Check if the dash duration has ended
    if (isDashing) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration<double>(now - dashStartTime).count() > dashDuration) {
            isDashing = false;
            dx = 0; // Reset dx after dash
        }
    }

    // Handle regular movement input if not dashing
    if (!isDashing) {
        if (keystate[SDL_SCANCODE_LEFT]) {
            dx = -speed;
        }
        else if (keystate[SDL_SCANCODE_RIGHT]) {
            dx = speed;
        }
        else {
            dx = 0;
        }

        // Handle jumping
        if (keystate[SDL_SCANCODE_SPACE] && isOnGround) {
            dy = jumpVelocity;  // Start jumping by setting the vertical velocity
            isJumping = true;
            isOnGround = false;
        }

        if (keystate[SDL_SCANCODE_R]) {  // Start recording with F5
            gameState.queueEvent(
                std::make_shared<StartRecordingEvent>(
                    gameState.getTimeline(),
                    sprite
                )
            );
        }
        if (keystate[SDL_SCANCODE_E]) {  // Stop recording with F6
            gameState.queueEvent(
                std::make_shared<StopRecordingEvent>(
                    gameState.getTimeline()
                )
            );
        }

        if (keystate[SDL_SCANCODE_L]) {  // Play replay with F7
            gameState.queueEvent(
                std::make_shared<PlayReplayEvent>(
                    gameState.getTimeline(),
                    sprite
                )
            );
            /*gameState.getEventManager().raiseEvent(std::make_shared<PlayReplayEvent>(
                gameState.getTimeline(),
                sprite
            ));*/
            std::cout << "Play replay event queued" << std::endl;
        }
    }
}