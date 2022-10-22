//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.19 Traits
//
//  22/10/20
//=============================================================================

#include <iostream>
#include <iterator>
#include <vector>

namespace { //=================================================================

class BigInt {
public:
    BigInt(long long)
    {}
};

template <typename T>
struct AccumulateTraits;

template <>
struct AccumulateTraits<char> {
    using ReturnType = int;
    static const ReturnType zero = 0;
};

template <>
struct AccumulateTraits<short> {
    using ReturnType = int;
    static const ReturnType zero = 0;
};

template <>
struct AccumulateTraits<int> {
    using ReturnType = long;
    static const ReturnType zero = 0;
};

template <>
struct AccumulateTraits<unsigned int> {
    using ReturnType = unsigned long;
    static const ReturnType zero = 0;
};

template <>
struct AccumulateTraits<float> {
    using ReturnType = double;
    static constexpr ReturnType zero = 0.0;
};

template <>
struct AccumulateTraits<BigInt> {
    using ReturnType = BigInt;
    inline static const BigInt zero = 0;
};

template <typename T0, typename T1>
class Multiply {
public:
    static void accumulate(T0& total, const T1& value) {
        total *= value;
    }
    static constexpr int initValue = 1;
};

template <typename T0, typename T1>
class Sum {
public:
    static void accumulate(T0& total, const T1& value) {
        total += value;
    }
    static constexpr int initValue = 0;
};

namespace _1 {

template <typename T,
          template <typename, typename> class Policy = Sum,
          typename Traits = AccumulateTraits<T>>
auto accum(const T* b, const T* e)
{
    using ReturnType = typename  Traits::ReturnType;
    ReturnType total = Policy<ReturnType, T>::initValue;

    while (b != e) {
        Policy<ReturnType, T>::accumulate(total, *b);
        ++b;
    }
    return total;
}



} //_1 --------------------------------------------------------------

namespace _2 {

template <typename Iter>
auto accum(Iter b, Iter e){
    using ValueType = typename std::iterator_traits<Iter>::value_type;

    auto total = ValueType{};
    while (b != e) {
        total += *b;
        ++b;
    }
    return total;
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    {
        int num[] = {1, 2, 3, 4, 5};
        auto total = _1::accum<int, Multiply>(num+0, num+5);
        std::cout << "_1::accum(): " << total << "\n";
    }
    {
        char str[] = "template";
        auto len = sizeof(str) - 1;
        auto total = _1::accum(str, str+len);
        std::cout << "_1::accum(" << str << "): " << (int)total << "\n";
    }
    {
        auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
        auto total = _2::accum(std::begin(v), std::end(v));
        std::cout << "_2::accum(): " << total << "\n";
    }
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_19_type_function();
void test_ch_19_sfinae_traits();

void test_ch_19()
{
#if (0) //done
    _3::test();
#endif
    test_ch_19_sfinae_traits();
}
