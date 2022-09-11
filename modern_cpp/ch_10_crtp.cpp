#include <iostream>
#include <vector>
#include <memory>

namespace { //=================================================================

class ControlBase {
public:
    virtual ~ControlBase()
    {}
    virtual void draw() = 0;
};

template <typename T>
class Control : public ControlBase {
    T* derived() {
        return static_cast<T*>(this);
    }
public:
    void draw() override {
        derived()->eraseBackground();
        derived()->paint();
    }
};

class Button : public Control<Button> {
public:
    void eraseBackground() {
        std::cout << "Button::eraseBackground()\n";
    }
    void paint() {
        std::cout << "Button::paint()\n";
    }
};

class Checkbox : public Control<Checkbox> {
public:
    void eraseBackground() {
        std::cout << "Checkbox::eraseBackground()\n";
    }
    void paint() {
        std::cout << "Checkbox::paint()\n";
    }
};

template <typename T>
void drawControl(Control<T>& c) {
    c.draw();
}

} //namespace =================================================================

// Curiously Recurring Template Pattern
void test_ch_10_crtp()
{
#if (0)
    auto b = Button{};
    drawControl(b);

    auto c = Checkbox{};
    drawControl(c);
#else
    auto v = std::vector<std::shared_ptr<ControlBase>>{};
    v.emplace_back(std::make_shared<Button>());
    v.emplace_back(std::make_shared<Checkbox>());

    for (auto& c : v) {
        c->draw();
    }
#endif
}
