#pragma once

#include <string>
#include <memory>
#include <functional>

#include "PositionComponent.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "DrawerComponent.h"
#include "TextureComponent.h"
#include "Timeline.h"

class NamedSprite {
private:
    std::string name;
    size_t typeHash;
    PositionComponent* position;
    MovementComponent* movement;
    InputComponent* input;
    DrawerComponent* drawer;
    TextureComponent* textureComponent;
    Timeline& localTimeline;
    double spawnX;
    double spawnY;

public:
    NamedSprite(const std::string& name, size_t typeHash, PositionComponent* position, MovementComponent* movement,
        InputComponent* input, DrawerComponent* drawer, Timeline& timeline, double spawnX, double spawnY);

    ~NamedSprite();

    void setTexture(TextureComponent* texture) { textureComponent = texture; }
    TextureComponent* getTexture() const { return textureComponent; }

    std::string getName() const;
    size_t getTypeHash() const;
    Timeline& getTimeline();
    PositionComponent* getPosition() const;
    MovementComponent* getMovement() const;
    InputComponent* getInput() const;
    DrawerComponent* getDrawer() const;
    double getSpawnX() const;
    double getSpawnY() const;
};
