#include "Shape.h"
#include "PhysicsEngine.h"

Shape::Shape(const std::string& name, int x, int y, int width, int height)
    : name(name), x(x), y(y), width(width), height(height), lastX(x), lastY(y) {}

const std::string& Shape::getName() const {
    return name;
}

int Shape::getX() const {
    return x;
}

int Shape::getY() const {
    return y;
}

int Shape::getWidth() const {
    return width;
}

int Shape::getHeight() const {
    return height;
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

void Shape::setX(int newX) {
    lastX = x;
    x = newX;
}

void Shape::setY(int newY) {
    lastY = y;
    y = newY;
}
