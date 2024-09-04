// Game.cpp
#include "Game.h"

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
