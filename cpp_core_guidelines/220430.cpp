#include <iostream>
#include <cstring>
#include <variant>

namespace { //=================================================================

// C166. Overload unary & only as part of a system of smart pointers and references
template <typename T>
class Ptr
{
    T* p;
public:
    Ptr(T* p) : p(p)
    {}
    T* operator->() { return p; }
    T operator[](int i);
    T operator*();
};

class X
{
public:
    Ptr<X> operator&() { return Ptr<X>{this}; }

    void print()
    {
        std::cout << "X::print()" << std::endl;
    }
    std::string str() const
    {
        return std::string{"Hello, Qt6"};
    }
};

// C167. Use an operator for an operation with its conventional meaning

std::ostream& operator<<(std::ostream& os, const X& x)
{
    return os << x.str();
}

// C168. Define overload operators in the namespace of their operands

// C170. If you feel like overloading a lambda,  use a generic lambda

// C180. Use a unions to save memory

constexpr size_t BufferSize = 16;

class ImmutalbeString
{
    union {
        char *p;
        char buf[BufferSize];
    };
    const size_t size;
public:
    ImmutalbeString(const char* str) : size(strlen(str))
    {
        if (size < BufferSize) {
            strcpy(buf, str);
        } else {
            p = new char [size + 1];
            strcpy(p, str);
        }
    }
   ~ImmutalbeString()
    {
        if (size >= BufferSize) {
            std::cout << "delete [] " << p << std::endl;
            delete [] p;
        }
    }
    const char* c_str() const
    {
        if (size < BufferSize)
            return buf;
        else
            return p;
    }
};

// C181. Avoid "naked" unions

// C182. Use anonymous unions to implement tagged unions

class Value
{
    enum class Tag {
        Number,
        Text
    };
    Tag type;
    union {
        int i;
        std::string s;
    };

public:
    struct BadEntry
    {};

   ~Value()
    {
        if (type == Tag::Text)
            s.~basic_string();
    }

    Value(const Value&) = default;
    Value& operator=(const Value& e)
    {
        if (type == Tag::Text && e.type == Tag::Text) {
            s = e.s;
            return *this;
        }
        if (type == Tag::Text)
            s.~basic_string();

        switch (e.type) {
        case Tag::Number:
            i = e.i;
            break;
        case Tag::Text:
            new(&s) std::string(e.s);
            break;
        }

        type = e.type;
        return *this;
    }

    int number() const
    {
        if (type == Tag::Text)
            throw BadEntry{};
        return i;
    }
    std::string text() const
    {
        if (type == Tag::Number)
            throw BadEntry{};
        return s;
    }
    void setNumber(int value)
    {
        if (type == Tag::Text) {
            s.~basic_string();
        }
        type = Tag::Number;
        i = value;
    }
    void setText(const std::string& value)
    {
        if (type == Tag::Text) {
            s = value;
        } else {
            new(&s) std::string{value};
        }
        type = Tag::Text;
        s = value;
    }
};

// C183. Don't use a union for type punning
union Pun
{
    int x;
    unsigned char c[sizeof(int)];
};
void badPun(Pun& u)
{
    u.x = 'x';
    std::cout << u.c[0] << std::endl;
}
void goodPun(int& x)
{
    auto p = reinterpret_cast<char*>(&x);
    std::cout << p[0] << std::endl;
}

} // namespace ================================================================

void test_c_166()
{
    X x;
    auto ptr = &x;

    ptr->print();

    std::cout << x << std::endl;
}

void test_c_180()
{
    auto s0 = ImmutalbeString("Hello, Qt6");
    std::cout << s0.c_str() << std::endl;

    auto s1 = ImmutalbeString("https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ro-custom");
    std::cout << s1.c_str() << std::endl;
}

void test_c_181()
{
    std::variant<int, float> v, w;
    v = 42;
    auto i = std::get<int>(v);
    assert(i == 42);

    w = std::get<int>(v);
    w = std::get<0>(v);
    w = v;

    try {
        std::get<float>(w);
    }  catch (const std::exception& e) {
        //bad_variant_access
        std::cout << e.what() << std::endl;
    }

    using namespace std::literals;

    std::variant<std::string> x("abc");
    x = "def";

    std::variant<std::string, void const*> y("abc");
    assert(std::holds_alternative<void const*>(y));
    y = "xyz"s;
    assert(std::holds_alternative<std::string>(y));
}

void test_c_183()
{
    Pun p;
    badPun(p);

    int i = 'x';
    goodPun(i);
}
