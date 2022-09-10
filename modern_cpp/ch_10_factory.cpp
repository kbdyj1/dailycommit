#include <iostream>
#include <memory>
#include <map>
#include <functional>

namespace { //=================================================================

class Image {
public:
    Image() { std::cout << "Image()\n"; }
    virtual ~Image()
    {}
};

class Bitmap : public Image {
public:
    Bitmap() { std::cout << "Bitmap()\n"; }
    ~Bitmap()
    {}
};

class Jpeg : public Image {
public:
    Jpeg() { std::cout << "Jpeg()\n"; }
    ~Jpeg()
    {}
};

class Png : public Image {
public:
    Png() { std::cout << "Png()\n"; }
    ~Png()
    {}
};

struct IImageFactory {
    virtual std::shared_ptr<Image> create(std::string_view format) = 0;
};

struct ImageFactory : public IImageFactory {
    std::shared_ptr<Image> create(std::string_view format) override {
#if (0)
        if (format == "bmp") {
            return std::make_shared<Bitmap>();
        } else if (format == "png") {
            return std::make_shared<Png>();
        } else if (format == "jpg") {
            return std::make_shared<Jpeg>();
        } else {
            return nullptr;
        }
#else
        static auto m = std::map<std::string, std::function<std::shared_ptr<Image>()>>{
            { "bmp", [](){ return std::make_shared<Bitmap>(); } },
            { "png", [](){ return std::make_shared<Png>(); } },
            { "jpg", [](){ return std::make_shared<Jpeg>(); } },
        };
        auto iter = m.find(format.data());
        if (iter != m.end()) {
            return iter->second();
        } else {
            return nullptr;
        }
#endif
    }
};

struct ImageFactoryByType {
    std::shared_ptr<Image> create(const std::type_info& format) {
        auto m = std::map<const std::type_info*, std::function<std::shared_ptr<Image>()>>{
            { &typeid(Bitmap), [](){ return std::make_shared<Bitmap>(); } },
            { &typeid(Png), [](){ return std::make_shared<Png>(); } },
            { &typeid(Jpeg), [](){ return std::make_shared<Jpeg>(); } },
        };
        auto iter = m.find(&format);
        if (iter != m.end()) {
            return iter->second();
        } else {
            return nullptr;
        }
    }
};

void test_factory()
{
#if (0)
    auto imageFactory = ImageFactory{};
    auto bmp = imageFactory.create("bmp");
    auto png = imageFactory.create("png");
    auto jpg = imageFactory.create("jpg");
#else
    auto imageFactory = ImageFactoryByType{};
    auto bmp = imageFactory.create(typeid(Bitmap));
    auto png = imageFactory.create(typeid(Png));
    auto jpg = imageFactory.create(typeid(Jpeg));
#endif
}

} //namespace =================================================================

void test_ch_10_factory()
{
    test_factory();
}
