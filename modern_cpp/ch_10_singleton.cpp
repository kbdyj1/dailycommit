#include <iostream>
#include <mutex>

namespace { //=================================================================

namespace test {

template <typename T>
class SingletonBase {
protected:
    SingletonBase()
    {}

public:
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

    static T& instance() {
        static T single;
        return single;
    }
};

class ImageLoader : public SingletonBase<ImageLoader> {
    ImageLoader()
    {}
    friend class SingletonBase<ImageLoader>;

public:
    void run() {
        std::cout << "ImageLoader::run()\n";
    }
};

} //test ------------------------------------------------------------

namespace dclp {

class Singleton {
    static Singleton* volatile single;
    static std::mutex mutex;

    Singleton()
    {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
    ~Singleton()
    {}

public:
    static Singleton* volatile instance() { // double checked locking pattern
        if (single == nullptr) {
            auto lock = std::lock_guard<std::mutex>(mutex);
            if (single == nullptr) {
                single = new Singleton();
            }
        }
        return single;
    }

    void run() {
        std::cout << "dclp::Singleton::run()\n";
    }
};

 Singleton* volatile Singleton::single = nullptr;
 std::mutex Singleton::mutex;

} //dclp ------------------------------------------------------------

} //namespace =================================================================

void test_ch_10_singleton()
{
#if (0)
    test::ImageLoader::instance().run();
#else
    dclp::Singleton::instance()->run();
#endif
}
