#include <iostream>

namespace { //=================================================================

class ControlProperty {
    int mId;
    std::string mText;
    int mWidth;
    int mHeight;
    bool mVisible;

    friend class Control;

public:
    ControlProperty(const int id) : mId(id)
    {}

    ControlProperty& text(std::string_view text) {
        mText = text.data();
        return *this;
    }
    ControlProperty& width(const int w) {
        mWidth = w;
        return *this;
    }
    ControlProperty& height(const int h) {
        mHeight = h;
        return *this;
    }
    ControlProperty& visible(const bool b) {
        mVisible = b;
        return *this;
    }
};

class Control {
    int mId;
    std::string mText;
    int mWidth;
    int mHeight;
    bool mVisible;

public:
    Control(int id, std::string_view text = "", const int w = 0, const int h = 0, const bool visible = false)
        : mId(id)
        , mText(text)
        , mWidth(w)
        , mHeight(h)
        , mVisible(visible)
    {}
    Control(const ControlProperty& properties)
        : mId(properties.mId)
        , mText(properties.mText)
        , mWidth(properties.mWidth)
        , mHeight(properties.mHeight)
        , mVisible(properties.mVisible)
    {}

    void print() {
        std::cout << "Control(id:" << mId
                  << ", w:" << mWidth
                  << ", h:" << mHeight
                  << ", text: " << mText
                  << ", visible: " << mVisible
                  << ")\n";
    }
};

} //namespace =================================================================

void test_ch_10_named_parameter()
{
    auto c = Control(
                ControlProperty(100)
                    .visible(true)
                    .width(1920)
                    .height(720)
                    .text("Hello, Qt")
             );

    c.print();
}
