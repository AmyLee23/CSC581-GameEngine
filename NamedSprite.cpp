#include "NamedSprite.h"

NamedSprite::NamedSprite(const std::string& name, PositionComponent* position, MovementComponent* movement,
    InputComponent* input, DrawerComponent* drawer, Timeline& timeline)
    : name(name), position(position), movement(movement), input(input), drawer(drawer), localTimeline(timeline) {}

NamedSprite::~NamedSprite() {
    delete position;
    delete movement;
    //delete input;
    delete drawer;
}

std::string NamedSprite::getName() const {
    return name;
}

Timeline& NamedSprite::getTimeline() {
    return localTimeline;
}

PositionComponent* NamedSprite::getPosition() const {
    return position;
}

void NamedSprite::setPosition(PositionComponent* newPosition) {
    position = newPosition;
}

MovementComponent* NamedSprite::getMovement() const {
    return movement;
}

InputComponent* NamedSprite::getInput() const {
    return input;
}

DrawerComponent* NamedSprite::getDrawer() const {
    return drawer;
}

//NamedSprite::NamedSprite(const std::string& name, std::unique_ptr<PositionComponent> position,
//    std::unique_ptr<MovementComponent> movement, std::unique_ptr<InputComponent> input, std::unique_ptr<DrawerComponent> drawer, Timeline& timeline)
//    : name(name), position(std::move(position)), movement(std::move(movement)), input(std::move(input)), drawer(std::move(drawer)), localTimeline(timeline) {}
//
//std::string NamedSprite::getName() const {
//    return name;
//}
//
//Timeline& NamedSprite::getTimeline() {
//    return localTimeline;
//}
//
//PositionComponent* NamedSprite::getPosition() const {
//    return position.get();  // Return raw pointer from smart pointer
//}
//
//MovementComponent* NamedSprite::getMovement() const {
//    return movement.get();  // Return raw pointer from smart pointer
//}
//
//InputComponent* NamedSprite::getInput() const {
//    return input.get();  // Return raw pointer from smart pointer
//}
//
//DrawerComponent* NamedSprite::getDrawer() const {
//    return drawer.get();  // Return raw pointer from smart pointer
//}