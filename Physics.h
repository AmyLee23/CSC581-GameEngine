#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

class Physics {
public:
    Physics(float gravity = 0.5f);

    void applyGravity(float& velocityY);

private:
    float gravity_;
};

#endif // PHYSICS_H
