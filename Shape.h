#pragma once

#include <string>
#include <unordered_map>
#include "Drawer.h"

class PhysicsEngine; // forward decleration

class Shape {
public:
    Shape(const std::string& name, float x, float y, int width, int height, Drawer& drawer);
    virtual ~Shape() = default;

    virtual void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics, float dt) = 0;
    void moveBack();
    void moveBackX();
    void moveBackY();

    const std::string& getName() const;
    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;

    void setX(float newX);
    void setY(float newY);
    void setWidth(int newWidth);
    void setHeight(int newHeight);

    Drawer& getDrawer();

protected:
    std::string name;
    float x, y;
    int width, height;
    float lastX, lastY;
    Drawer& drawer;
};
