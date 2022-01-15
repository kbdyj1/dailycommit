#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QProperty>

class Rectangle
{
    QProperty<int> mWidth { 10 };
    QProperty<int> mHeight{ 20 };
    QProperty<int> mArea;

public:
    Rectangle() {
        mArea.setBinding([&]{
            return mWidth * mHeight;
        });
    }

    QBindable<int> bindableArea() {
        return &mArea;
    };
    int area() const {
        return mArea.value();
    }
    void setWidth(int w) {
        mWidth = w;
    }
    void setHeight(int h) {
        mHeight = h;
    }
};

#endif // RECTANGLE_H
