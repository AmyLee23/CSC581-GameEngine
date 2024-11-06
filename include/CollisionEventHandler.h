#pragma once
#include "GameState.h"
#include "CollisionEvent.h"
//class CollisionEventHandler : public EventHandler {
//public:
//    void onEvent(std::shared_ptr<Event> event) override {
//        auto collisionEvent = std::static_pointer_cast<CollisionEvent>(event);
//        auto& sprite = collisionEvent->getSprite();
//
//        if (auto movement = sprite.getMovement()) { 
//            // Set isOnGround if moving down
//            if (collisionEvent->getWasMovingDown()) {
//                collisionEvent->getIsOnGround() = true;
//                sprite.getPosition()->setY(collisionEvent->getLastValidY());
//                return;
//            }
//            movement->setDy(0);  // Stop vertical movement
//            sprite.getPosition()->setY(collisionEvent->getLastValidY());            
//        }
//    }
//};
class CollisionEventHandler : public EventHandler {
public:
    void onEvent(std::shared_ptr<Event> event) override {
        auto collisionEvent = std::static_pointer_cast<CollisionEvent>(event);
        auto& sprite = collisionEvent->getSprite();

        if (auto movement = sprite.getMovement()) {
            if (collisionEvent->isHorizontalCollision()) {
                // Handle horizontal collision
                sprite.getPosition()->setX(collisionEvent->getLastValidX());
                return;
            }

            // Handle vertical collision (existing code)
            if (collisionEvent->getWasMovingDown()) {
                collisionEvent->getIsOnGround() = true;
                sprite.getPosition()->setY(collisionEvent->getLastValidY());
                return;
            }
            movement->setDy(0);
            sprite.getPosition()->setY(collisionEvent->getLastValidY());
        }
    }
};