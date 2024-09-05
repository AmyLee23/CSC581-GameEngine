#pragma once
#ifndef INIT_H
#define INIT_H

#include <SDL.h>
const float INITIAL_WIDTH = 800;  // Initial window width
const float INITIAL_HEIGHT = 600; // Initial window height
const float SQUARE_SIZE = 50;
const float SQUARE_SPEED = 1;
const float Rect_SIZE = 200;


bool init(SDL_Window*& window, SDL_Renderer*& renderer);
void getWindowSize(SDL_Window* window, int& width, int& height);

#endif // INIT_H
