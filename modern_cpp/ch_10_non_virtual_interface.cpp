#include <iostream>
#include <vector>
#include <memory>

namespace { //=================================================================

class Control {
public:
    virtual ~Control() {
        std::cout << "~Control()\n";
    }

private:
    virtual void paint() = 0;

protected:
    virtual void eraseBackground() {
        std::cout << "Control::eraseBackground()\n";
    }

public:
    void draw() {
        eraseBackground();
        paint();
    }
};

class Button : public Control {
    void paint() override {
        std::cout << "Button::paint()\n";
    }
protected:
    void eraseBackground() override {
        Control::eraseBackground();
        std::cout << "Button::eraseBackground()\n";
    }
public:
    virtual ~Button() {
        std::cout << "~Button()\n";
    }
};

class Checkbox : public Button {
    void paint() override {
        std::cout << "Checkbox::paint()\n";
    }
protected:
    void eraseBackground() override {
        Button::eraseBackground();
        std::cout << "Checkbox::eraseBackground()\n";
    }
public:
    virtual ~Checkbox() {
        std::cout << "~Checkbox()\n";
    }
};

} //namespace =================================================================

void test_ch_10_non_virtual_interface()
{
    auto v = std::vector<std::shared_ptr<Control>>{};

    v.push_back(std::make_shared<Button>());
    v.push_back(std::make_shared<Checkbox>());

    for (const auto& c : v) {
        c->draw();
        std::cout << "--------------------------------------------\n";
    }

    std::cout << "button->draw()\n";
    auto button = std::make_shared<Button>();
    button->draw();
}
