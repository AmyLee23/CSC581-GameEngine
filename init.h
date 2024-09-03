#pragma once
#ifndef INIT_H
#define INIT_H

#include <SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SQUARE_SIZE = 50;
const int SQUARE_SPEED = 1;

bool init(SDL_Window*& window, SDL_Renderer*& renderer);

#endif // INIT_H
