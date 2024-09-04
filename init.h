#pragma once
#ifndef INIT_H
#define INIT_H

#include <SDL.h>
const int SQUARE_SIZE = 50;
const int SQUARE_SPEED = 1;

bool init(SDL_Window*& window, SDL_Renderer*& renderer);
void getWindowSize(SDL_Window* window, int& width, int& height);

#endif // INIT_H
