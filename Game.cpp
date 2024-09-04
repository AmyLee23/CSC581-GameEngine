// Game.cpp
#include "Game.h"
#include "ControllableShape.h"
#include <iostream>

Game::Game(WindowManager& windowManager)
    : physics(windowManager) {
    lastTime = std::chrono::steady_clock::now();
}

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
    // handle time change in seconds
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastTime;
    float dt = elapsedTime.count();
    lastTime = currentTime;

    //std::cout << "Delta Time (dt): " << dt << " seconds" << std::endl;

    // handle input
    for (const auto& pair : getShapes()) {
        if (auto controllable = dynamic_cast<ControllableShape*>(pair.second)) {
            controllable->handleInput(keystate, this, dt);
        }
    }
    // move each shape accordingly
    for (const auto& pair : shapes) {
        Shape* shape = pair.second;
        shape->move(shapes, physics, dt); // move the shape while checking collisions
    }
}
