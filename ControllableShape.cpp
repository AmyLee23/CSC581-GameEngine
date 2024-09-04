#include "ControllableShape.h"
#include "PhysicsEngine.h"
#include "Game.h"

ControllableShape::ControllableShape(const std::string& name, int x, int y, int width, int height)
    : Shape(name, x, y, width, height) {}

void ControllableShape::move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) {
    // do nothing as movement is controlled by input    
}

void ControllableShape::handleInput(const Uint8* keystate, Game& game) {
    lastX = x;
    lastY = y;
    PhysicsEngine& physics = game.getPhysicsEngine();

    if (keystate[SDL_SCANCODE_UP]) {
        y -= speed;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        y += speed;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        x -= speed;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        x += speed;
    }

    if (physics.checkShapeCollision(this, game.getShapes())) {
        moveBack();
    }
}

void ControllableShape::setSpeed(int speed) {
    speed = speed;
}

int ControllableShape::getSpeed() {
    return speed;
}

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
