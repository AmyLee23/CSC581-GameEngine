// Game.cpp
#include "Game.h"
#include "ControllableShape.h"

Game::Game(WindowManager& windowManager)
    : physics(windowManager) {}

Game::~Game() {
    // Clean up all shapes
    for (auto& pair : shapes) {
        delete pair.second;
    }
}

void Game::addShape(Shape* shape) {
    shapes[shape->getName()] = shape;
}

Shape* Game::getShape(const std::string& name) const {
    auto it = shapes.find(name);
    if (it != shapes.end()) {
        return it->second;
    }
    return nullptr;
}

const std::unordered_map<std::string, Shape*>& Game::getShapes() const {
    return shapes;
}


void Game::handleCollisions() {
    for (auto& pair : shapes) {
        Shape* shape = pair.second;
        if (physics.checkShapeCollision(shape, shapes)) {
            shape->moveBack();  // move back if collision detected
        }
    }
}

PhysicsEngine& Game::getPhysicsEngine() {
    return physics;
}

void Game::step(const Uint8* keystate) {
    // handle input
    for (const auto& pair : getShapes()) {
        if (auto controllable = dynamic_cast<ControllableShape*>(pair.second)) {
            controllable->handleInput(keystate, this);
        }
    }
    // move each shape accordingly
    for (const auto& pair : shapes) {
        Shape* shape = pair.second;
        shape->move(shapes, physics); // move the shape while checking collisions
    }
}
