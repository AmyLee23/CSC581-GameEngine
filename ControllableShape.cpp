#include "ControllableShape.h"
#include "PhysicsEngine.h"
#include "Game.h"

ControllableShape::ControllableShape(const std::string& name, float x, float y, int width, int height, Drawer& drawer, SDL_Color color, float dx, float dy, bool flying)
    : Shape(name, x, y, width, height, drawer, color), dx(dx), dy(dy), flying(flying) {}


void ControllableShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) {
    lastX = x;
    lastY = y;

    if (!physics.checkBottomCollision(this, shapes)) {
        isOnGround = false;
    }
    if (!flying && !isOnGround) {
        physics.applyGravity(this, dt);
        //if (physics.checkSideCollision(this, shapes)) {
        //    moveBackX();
        //}
        if (physics.checkBottomCollision(this, shapes)) {
            moveBackY();
            isOnGround = true;
            dy = 0;
        }
        else if (physics.checkTopBottomCollision(this, shapes)) {
            dy = 0;
            moveBackY();
        } 
    }
}

void ControllableShape::handleInput(const Uint8* keystate, Game* game, float dt) {
    lastX = x;
    lastY = y;
    PhysicsEngine& physics = game->getPhysicsEngine();

    float metersMovedX = dx * dt;  // Distance in meters (speed * time)
    float metersMovedY = dy * dt;
    float moveX = metersMovedX * physics.getPixelsPerMeter();
    float moveY = metersMovedY * physics.getPixelsPerMeter();

    if (flying) {
        if (keystate[SDL_SCANCODE_UP]) {
            y -= moveY;
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            y += moveY;
        }
    }

    // Handle jumping
    if (keystate[SDL_SCANCODE_SPACE] && isOnGround) {
        dy = jumpVelocity;  // start jumping by setting the vertical velocity
        isJumping = true;
        isOnGround = false;
    }
    
    if (keystate[SDL_SCANCODE_LEFT]) {
        x -= moveX;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        x += moveX;
    }

    if (physics.checkSideCollision(this, game->getShapes())) {
        moveBackX();
    }
    if (physics.checkTopBottomCollision(this, game->getShapes())) {
        moveBackY();
    }
}

void ControllableShape::setSpeed(float newDx, float newDy) { dx = newDx; dy = newDy; }
std::tuple<float, float> ControllableShape::getSpeed() const { return std::make_tuple(dx, dy); }

float ControllableShape::getJumpVelocity() { return jumpVelocity; }
void ControllableShape::setJumpVelocity(float newJumpVelocity) { jumpVelocity = newJumpVelocity; }


bool ControllableShape::getIsJumping() { return isJumping; }
bool ControllableShape::getIsOnGround() { return isOnGround; }
bool ControllableShape::getFlying() { return flying; }

void ControllableShape::setIsJumping(bool jump) { isJumping = jump; }
void ControllableShape::setIsOnGround(bool ground) { isOnGround = ground; }
void ControllableShape::setFlying(bool flying) { this->flying = flying; }



/*
void ControllableShape::handleEvent(const SDL_Event& e) {
    lastX = x;
    lastY = y;

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:    y -= speed; break;
        case SDLK_DOWN:  y += speed; break;
        case SDLK_LEFT:  x -= speed; break;
        case SDLK_RIGHT: x += speed; break;
        }
    }
}*/
