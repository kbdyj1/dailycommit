//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.12 Template details
//
//  22/09/29
//=============================================================================

#include <iostream>

namespace { //=================================================================

template <typename T>
class Data {
public:
    static constexpr bool copyable = true;
};

template <typename T>
void log(T message)
{
    std::cout << message << "\n";
}

template <typename T>
T zero = 0;

template <typename T>
bool dataCopyable = Data<T>::copyable;

template <typename T>
using DataList = Data<T*>;

class Collection {
public:
    template <typename T>
    class Node {
    };

    template <typename T>
    T* alloc() {
        return nullptr;
    }

    template <typename T>   //c++14
    static T zero = 0;

    template <typename T>
    using NodePtr = Node<T>*;
};


template <typename T>
class Array {
};

class Value {
public:
    explicit Value(int)
    {}
};

template <typename T>
void fill(Array<T>& array, const T& = T())
{}

void init(Array<Value>& array)
{
    auto zero = Value{0};

    fill(array, zero);
    //fill(array);    // no matching constructor for initialization of '(anoymous namespace::Value'
}

void test_default_constructor()
{
    auto array = Array<Value>{};

    init(array);
}

//-------------------------------------------------------------------

#if 201402L < __cplusplus   //c++17
#   define USE_INLINE
#endif

template <typename T>
class Board {
    class Internal;

public:
    void open();
    enum Type : unsigned char;

#if defined(USE_INLINE)
    inline static double totalWeight = 0.0;
#else
    static double totalWeight;
#endif
};

template <typename T>
class Board<T>::Internal {
    T value;
public:
    Internal() : value(T{})
    {}

    T getValue() const {
        return value;
    }
};

template <typename T>
void Board<T>::open()
{
    std::cout << "Board<T>::open()\n";
    std::cout << "totalWeight: " << totalWeight << "\n";
}

template <typename T>
enum Board<T>::Type : unsigned char {
    Normal = 0,
    Premium
};

#if !defined(USE_INLINE)
template <typename T>
double Board<T>::totalWeight = 0.0;
#endif

void test_board()
{
    auto board = Board<int>{};
    board.open();
}

//-------------------------------------------------------------------

template <typename T>
class Dynamic {
public:
    virtual ~Dynamic()
    {}

    template <typename U>
#if (0)
    virtual void copy(const U&);
#else
    void copy(const U&);
#endif
};

struct {
    template <typename T>
    void f()
    {
        std::cout << "struct {\n\tvoid f<" << typeid(T).name() << ">()\n}\n";
    }
} unnamed;

void test_unnamed_structure()
{
    unnamed.f<int>();
}

} //namespace =================================================================

extern "C++" template <typename T>
void cxx_template_linkage()
{
    std::cout << "extern C++ normal()\n";
}

#if (0)
extern "C" template <typename T>
void c_template_linkage()
{
    // templates must have c++ linkage
}
#endif

template <typename T>
void external();

void test_external_template()
{
    external<int>();
}

template <typename T>
constexpr T zero = T{};

const int intZero = int{};  // internal linkage

void test_ch_12_template_parameter();
void test_ch_12_template_parameter_pack();
void test_ch_12_friend();

void test_ch_12()
{
#if (0)
    test_default_constructor();
    test_board();
    test_external_template();
    test_unnamed_structure();
    test_ch_12_template_parameter();
    test_ch_12_template_parameter_pack();
#endif

    test_ch_12_friend();
}
