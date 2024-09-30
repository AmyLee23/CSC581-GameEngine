#pragma once

#include <string>
#include <memory>

#include "PositionComponent.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "DrawerComponent.h"
#include "Timeline.h"

class NamedSprite {
private:
    std::string name;
    PositionComponent* position;
    MovementComponent* movement;
    InputComponent* input;
    DrawerComponent* drawer;
    Timeline& localTimeline;

public:
    NamedSprite(const std::string& name, PositionComponent* position, MovementComponent* movement,
        InputComponent* input, DrawerComponent* drawer, Timeline& timeline);

    ~NamedSprite();

    std::string getName() const;
    Timeline& getTimeline();
    PositionComponent* getPosition() const;
    void setPosition(PositionComponent* newPosition);
    MovementComponent* getMovement() const;
    InputComponent* getInput() const;
    DrawerComponent* getDrawer() const;
};

//class NamedSprite {
//private:
//    std::string name;
//    std::unique_ptr<PositionComponent> position;
//    std::unique_ptr<MovementComponent> movement;
//    std::unique_ptr<InputComponent> input;
//    std::unique_ptr<DrawerComponent> drawer;
//    Timeline& localTimeline;  
//
//public:
//    NamedSprite(const std::string& name, std::unique_ptr<PositionComponent> position, std::unique_ptr<MovementComponent> movement, 
//        std::unique_ptr<InputComponent> input, std::unique_ptr<DrawerComponent> drawer, Timeline& timeline);
//
//    ~NamedSprite() = default;
//
//    std::string getName() const;
//    Timeline& getTimeline();
//    PositionComponent* getPosition() const;
//    MovementComponent* getMovement() const;
//    InputComponent* getInput() const;
//    DrawerComponent* getDrawer() const;
//};