//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.27 Expression template
//
//  22/11/10
//=============================================================================

#include <iostream>
#include <cassert>

template <typename T>
class SArray {
    T* data = nullptr;
    std::size_t dataSize = 0;

public:
    explicit SArray(std::size_t s) : data(new T[s]), dataSize(s)
    {}
    SArray(const SArray& other) : data(new T[other.size()]), dataSize(other.size())
    {}
    ~SArray()
    {
        delete [] data;
    }
    SArray& operator=(const SArray& other)
    {
        if (this != &other) {
            dataSize = other.size();
            data = new T[dataSize];
        }
        return *this;
    }
    std::size_t size() const
    {
        return dataSize;
    }
    const T& operator[](std::size_t index) const
    {
        return data[index];
    }
    T& operator[](std::size_t index)
    {
        return data[index];
    }

protected:
    void init() {
        for (std::size_t i = 0; i < size(); ++i) {
            data[i] = T();
        }
    }
    void copy(const SArray& other) {
        assert(size() == other.size());
        for (std::size_t i = 0; i < size(); ++i) {
            data[i] = other.data[i];
        }
    }
};

template <typename T>
SArray<T> operator+(const SArray<T>& l, const SArray<T>& r)
{
    assert(l.size() == r.size());
    auto result = SArray<T>(l.size());
    for (auto i=0; i<l.size(); i++) {
        result[i] = l[i] + r[i];
    }
    return result;
}

template <typename T>
SArray<T> operator*(const SArray<T>& l, const SArray<T>& r)
{
    assert(l.size() == r.size());
    auto result = SArray<T>(l.size());
    for (auto i=0; i<l.size(); i++) {
        result[i] = l[i] * r[i];
    }
    return result;
}

template <typename T>
SArray<T> operator*(const T& s, const SArray<T>& r)
{
    auto result = SArray<T>(r.size());
    for (auto i=0; i<r.size(); i++) {
        result[i] = r[i] * s;
    }
    return result;
}

template <typename T>
SArray<T> operator*(const SArray<T>& r, const T& s)
{
    return s * r;
}

namespace { //=================================================================

namespace _1 {

void test()
{
    auto l = SArray<int>(10);
    auto r = SArray<int>(10);
    for (auto i=0; i<10; i++) {
        l[i] = i;
        r[i] = i;
    }
    auto result = l * r;
    for (auto i=0; i<10; i++) {
        std::cout << result[i] << "\n";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
class Scalar;

template <typename T>
class Traits {
public:
    using ExpRef = const T&;
};

template <typename T>
class Traits<Scalar<T>> {
public:
    using ExpRef = Scalar<T>;
};

template <typename T, typename OP0, typename OP1>
class Add {
    typename Traits<OP0>::ExpRef op0;
    typename Traits<OP1>::ExpRef op1;

public:
    Add(const OP0& op0, const OP1& op1) : op0(op0), op1(op1)
    {}
    T operator[](std::size_t i) const {
        return op0[i] + op1[i];
    }
    std::size_t size() const {
        return op0.size() != 0 ? op0.size() : op1.size();
    }
};

template <typename T, typename OP0, typename OP1>
class Mul {
    typename Traits<OP0>::ExpRef op0;
    typename Traits<OP1>::ExpRef op1;

public:
    Mul(const OP0& op0, const OP1& op1) : op0(op0), op1(op1)
    {}
    T operator[](std::size_t i) const {
        return op0[i] * op1[i];
    }
    std::size_t size() const {
        return op0.size() != 0 ? op0.size() : op1.size();
    }
};

template <typename T>
class Scalar {
    const T& s;

public:
    constexpr Scalar(const T& v) : s(v)
    {}
    constexpr const T& operator[](std::size_t) const {
        return s;
    }
    constexpr std::size_t size() const {
        return 0;
    }
};

template <typename T, typename Rep = SArray<T>>
class Array {
    Rep exp;

public:
    explicit Array(std::size_t s) : exp(s)
    {}
    Array(const Rep& other) : exp(other)
    {}
    Array& operator=(const Array& other)
    {
        if (this != &other) {
            assert(size() == other.size());
            for (auto i=0; i<size(); i++) {
                exp[i] = other.exp[i];
            }
        }
        return *this;
    }

    template<typename T2, typename Rep2>
    Array& operator=(const Array<T2, Rep2>& other)
    {
        assert(size() != other.size());
        for (auto i=0; i<size(); i++) {
            exp[i] = other[i];
        }
        return *this;
    }

    std::size_t size() const
    {
        return exp.size();
    }

    decltype(auto) operator[](std::size_t i) const
    {
        assert(i < size());
        return exp[i];
    }
    T& operator[](std::size_t i)
    {
        assert(i < size());
        return exp[i];
    }
    const Rep& rep() const {
        return exp;
    }
    Rep& rep() {
        return exp;
    }
};

template <typename T, typename R0, typename R1>
Array<T, Add<T, R0, R1>> operator+(const Array<T, R0>& l, const Array<T, R1>& r)
{
    return Array<T, Add<T, R0, R1>>(Add<T, R0, R1>(l.rep(), r.rep()));
}

template <typename T, typename R0, typename R1>
Array<T, Mul<T, R0, R1>> operator*(const Array<T, R0>& l, const Array<T, R1>& r)
{
    return Array<T, Mul<T, R0, R1>>(Mul<T, R0, R1>(l.rep(), r.rep()));
}

template <typename T, typename R>
Array<T, Mul<T, Scalar<T>, R>> operator*(const T& s, const Array<T, R>& l)
{
    return Array<T, Mul<T, Scalar<T>, R>>(Mul<T, Scalar<T>, R>(Scalar<T>(s), l.rep()));
}


} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_27()
{
    _1::test();
}
