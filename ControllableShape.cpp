#include "ControllableShape.h"
#include "PhysicsEngine.h"
#include "Game.h"

ControllableShape::ControllableShape(const std::string& name, int x, int y, int width, int height, int dx, int dy, bool flying)
    : Shape(name, x, y, width, height), dx(dx), dy(dy), flying(flying) {}


void ControllableShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) {
    if (!flying && !isOnGround) {
        physics.applyGravity(this);
        if (physics.checkSideCollision(this, shapes)) {
            moveBackX();
        }
        else if (physics.checkBottomCollision(this, shapes)) {
            moveBackY();
            isOnGround = true;
        }
        else if (physics.checkTopBottomCollision(this, shapes)) {
            moveBackY();
        }
        
    }
}

void ControllableShape::handleInput(const Uint8* keystate, Game* game) {
    lastX = x;
    lastY = y;
    PhysicsEngine& physics = game->getPhysicsEngine();

    if (flying) {
        if (keystate[SDL_SCANCODE_UP]) {
            y -= dy;
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            y += dy;
        }
    }

    // Handle jumping
    if (keystate[SDL_SCANCODE_SPACE] && isOnGround) {
        dy = jumpVelocity;  // start jumping by setting the vertical velocity
        isJumping = true;
        isOnGround = false;
    }
    
    if (keystate[SDL_SCANCODE_LEFT]) {
        x -= dx;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        x += dx;
    }

    if (physics.checkSideCollision(this, game->getShapes())) {
        moveBackX();
    }
    if (physics.checkTopBottomCollision(this, game->getShapes())) {
        moveBackY();
    }
}

void ControllableShape::setSpeed(int newDx, int newDy) { dx = newDx; dy = newDy; }
std::tuple<int, int> ControllableShape::getSpeed() const { return std::make_tuple(dx, dy); }

int ControllableShape::getJumpVelocity() { return jumpVelocity; }
void ControllableShape::setJumpVelocity(int newJumpVelocity) { jumpVelocity = newJumpVelocity; }


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
