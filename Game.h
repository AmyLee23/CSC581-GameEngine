#pragma once

#include <unordered_map>
#include <string>

#include "Shape.h"
#include "PhysicsEngine.h"

class Game {
public:
    Game(WindowManager& windowManager);
    ~Game();

    void addShape(Shape* shape); // add a shape to the game by pointer
    Shape* getShape(const std::string& name) const; // get a shape by name

    const std::unordered_map<std::string, Shape*>& getShapes() const;

    void handleCollisions();

    PhysicsEngine& getPhysicsEngine();

private:
    std::unordered_map<std::string, Shape*> shapes; // store shapes by name

    PhysicsEngine physics;
};
