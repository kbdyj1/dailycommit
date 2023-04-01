#include <iostream>

namespace { //=================================================================

template <typename T>
struct Property
{
    T value;

    Property(const T& value) : value(value)
    {}

    operator T()
    {
        return value;
    }
    T operator=(const T& rhs)
    {
        return value = rhs;
    }
};

struct Enermy
{
    Property<int> power{10};
    Property<int> defense{5};
};

namespace _1 {

void test()
{
    auto e = Enermy{};
    e.defense = 10;
    std::cout << "enermy's power: " << e.power << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

struct Image
{
    virtual void draw() = 0;
};

struct Bitmap : Image
{
    Bitmap(const std::string& filename)
    {
        std::cout << "Loading image from: " << filename << "\n";
    }
    void draw() override
    {
        std::cout << "Bitmap::draw()\n";
    }
};

struct LazyBitmap : public Image
{
    Bitmap* bmp = nullptr;
    std::string filename;

public:
    LazyBitmap(const std::string& filename) : filename(filename)
    {}
    ~LazyBitmap()
    {
        delete  bmp;
    }
    void draw() override
    {
        if (!bmp)
            bmp = new Bitmap(filename);
        bmp->draw();
    }
};

} //_2 --------------------------------------------------------------

} // namespace ================================================================

void test_proxy()
{
    _1::test();
}
