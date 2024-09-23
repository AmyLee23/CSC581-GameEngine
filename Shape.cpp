#include "Shape.h"
#include "PhysicsEngine.h"

Shape::Shape(const std::string& name, float x, float y, int width, int height, Drawer& drawer, SDL_Color color)
    : name(name), x(x), y(y), width(width), height(height), lastX(x), lastY(y), drawer(drawer), color(color) {}

const std::string& Shape::getName() const {
    return name;
}

float Shape::getX() const {
    return x;
}

float Shape::getY() const {
    return y;
}

int Shape::getWidth() const {
    return width;
}

int Shape::getHeight() const {
    return height;
}

SDL_Color Shape::getColor() const {
    return color;
}

void Shape::moveBack() {
    x = lastX;
    y = lastY;
}

void Shape::moveBackX() {
    x = lastX;
}

void Shape::moveBackY() {
    y = lastY;
}

void Shape::setX(float newX) {
    lastX = x;
    x = newX;
}

void Shape::setY(float newY) {
    lastY = y;
    y = newY;
}

void Shape::setWidth(int newWidth) {
    width = newWidth;
}

void Shape::setHeight(int newHeight) {
    height = newHeight;
}

Drawer& Shape::getDrawer() {
    return drawer;
}

