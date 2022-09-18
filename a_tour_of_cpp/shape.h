#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>

struct Shape {
    virtual ~Shape(){}
    virtual void draw() = 0;
    virtual void rotate(double angle) = 0;
};
struct Rectangle : public Shape {
    ~Rectangle() {
        std::cout << "~Rectangle()\n";
    }
    void draw() override {
        std::cout << "Rectangle::draw()\n";
    }
    void rotate(double angle) override {
        std::cout << "Rectangle::rotate(" << angle << ")\n";
    }
};
struct Circle : public Shape {
    ~Circle() {
        std::cout << "~Circle()\n";
    }
    void draw() override {
        std::cout << "Circle::draw()\n";
    }
    void rotate(double angle) override {
        std::cout << "Circle::rotate(" << angle << ")\n";
    }
};

#endif // SHAPE_H
