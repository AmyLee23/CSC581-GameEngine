#include "Drawer.h"
#include "Game.h"
#include "Shape.h"

Drawer::Drawer() {}
Drawer::~Drawer() {}

void Drawer::drawShape(Game& game, Shape* shape, bool scaling) {
	SDL_Renderer* renderer = game.getWindowManager().getRenderer();

	//prepareRelativePosition(shape, game);
	//if (scaling) { prepareScaling(shape, game); }
	

    SDL_Rect rect = { static_cast<int>(shape->getX()), static_cast<int>(shape->getY()), shape->getWidth(), shape->getHeight() };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red color for shapes
    SDL_RenderFillRect(renderer, &rect);
}

void Drawer::prepareRelativePosition(Shape* shape, Game& game) {
	int windowWidth = game.getWindowManager().getWindowWidth();
	int windowHeight = game.getWindowManager().getWindowHeight();

	int lastWindowWidth = game.getWindowManager().getLastWindowWidth();
	int lastWindowHeight = game.getWindowManager().getLastWindowHeight();

	float x = shape->getX();
	float y = shape->getY();

	float relative_x = x / static_cast<float>(lastWindowWidth) * static_cast<float>(windowWidth);
	float relative_y = y / static_cast<float>(lastWindowHeight) * static_cast<float>(windowHeight);

	shape->setX(relative_x);
	shape->setY(relative_y);
}

void Drawer::prepareScaling(Shape* shape, Game& game) {
	int windowWidth = game.getWindowManager().getWindowWidth();
	int windowHeight = game.getWindowManager().getWindowHeight();
	int lastWindowWidth = game.getWindowManager().getLastWindowWidth();
	int lastWindowHeight = game.getWindowManager().getLastWindowHeight(); 

	float height = static_cast<float>(shape->getHeight());
	float width = static_cast<float>(shape->getWidth());

	float relative_height = height / static_cast<float>(lastWindowHeight) * static_cast<float>(windowHeight);
	float relative_width = width / static_cast<float>(lastWindowWidth) * static_cast<float>(windowWidth);

	shape->setHeight(static_cast<int>(relative_height));
	shape->setWidth(static_cast<int>(relative_width));
}