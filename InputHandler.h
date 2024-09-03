#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

void handleInput(bool& quit, float& dx, float& dy);
void applyGravity(float& dy);

// Optionally declare isJumping and isOnGround here if needed by other parts of the program
extern bool isJumping;
extern bool isOnGround;

#endif  // INPUTHANDLER_H

