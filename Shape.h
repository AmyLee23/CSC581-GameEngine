#pragma once

#include <string>
#include <unordered_map>

class PhysicsEngine; // forward decleration


class Shape {
public:
    Shape(const std::string& name, int x, int y, int width, int height);
    virtual ~Shape() = default;

    virtual void move(const std::unordered_map<std::string, Shape*>& shapes, PhysicsEngine& physics) = 0;
    void moveBack();
    void moveBackX();
    void moveBackY();

    const std::string& getName() const;
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;

    void setX(int newX);
    void setY(int newY);

protected:
    std::string name;
    int x, y;
    int width, height;
    int lastX, lastY;
};
