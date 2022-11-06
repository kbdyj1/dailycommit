//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.24 Type list
//
//  22/11/03
//=============================================================================

#include <iostream>
#include "TypeList.hpp"
#include "IfThenElse.hpp"

namespace { //=================================================================

using SignedIntegralTypes = TypeList<signed char, short, int, long, long long>;

namespace _1 { //----------------------------------------------------

template <typename List, int N>
class NthElementT : public NthElementT<PopFront<List>, N-1>
{};

template <typename List>
class NthElementT<List, 0> : public FrontT<List>
{};

template <typename List, int N>
using NthElemnt = typename NthElementT<List, N>::Type;

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename List, bool Empty = IsEmpty<List>::value>
class LargestTypeT;

template <typename List>
class LargestTypeT<List, false>
{
    using First = Front<List>;
    using Rest = typename LargestTypeT<PopFront<List>>::Type;

public:
    using Type = IfThenElse<(sizeof(First) >= sizeof(Rest)), First, Rest>;
};

template <typename List>
class LargestTypeT<List, true> {
public:
    using Type = char;
};

template <typename List>
using LargestType = typename LargestTypeT<List>::Type;

} //_2 --------------------------------------------------------------

namespace _3 {



} //_3 --------------------------------------------------------------

namespace _4 {

template <typename List, typename New, bool = IsEmpty<List>::value>
class PushBackRecT;

template <typename List, typename New>
class PushBackRecT<List, New, false> {
    using Head = Front<List>;
    using Tail = PopFront<List>;
    using NewTail = typename PushBackRecT<Tail, New>::Type;

public:
    using Type = PushFront<Head, NewTail>;
};

template <typename List, typename New>
class PushBackRecT<List, New, true> {
public:
    using Type = PushFront<List, New>;
};

template <typename List, typename New>
class PushBackT : public PushBackRecT<List, New>
{};

template <typename List, typename New>
using PushBack = typename PushBackT<List, New>::Type;

} //_4 --------------------------------------------------------------

namespace _5 {

using namespace _4;

template <typename List, bool Empty = IsEmpty<List>::value>
class ReverseT;

template <typename List>
using Reverse = typename ReverseT<List>::Type;

template <typename List>
class ReverseT<List, false> : public PushBackT<Reverse<PopFront<List>>, Front<List>>
{};

template <typename List>
class ReverseT<List, true>
{
public:
    using Type = List;
};

template <typename List>
class PopBackT {
public:
    using Type = Reverse<PopFront<Reverse<List>>>;
};

template <typename List>
using PopBack = typename PopBackT<List>::Type;

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
struct AddConstT {
    using Type = const T;
};

template <typename T>
using AddConst = typename AddConstT<T>::Type;

} //_6 --------------------------------------------------------------

namespace _7 {

template <typename List, template <typename T> class Func, bool Empty = IsEmpty<List>::value>
class TransformT;

template <typename List, template <typename T> class Func>
class TransformT<List, Func, false>
        : public PushFrontT<typename TransformT<PopFront<List>, Func>::Type, typename Func<Front<List>>::Type>
{};

} //_7 --------------------------------------------------------------

namespace _8 {

template <typename List, template<typename X, typename Y> class F, typename I, bool = IsEmpty<List>::value>
class AccumulateT;

template <typename List, template<typename X, typename Y> class F, typename I>
class AccumulateT<List, F, I, false> : public AccumulateT<PopFront<List>, F, typename F<I, Front<List>>::Type>
{};

template <typename List, template<typename X, typename Y> class F, typename I>
class AccumulateT<List, F, I, true>
{
public:
    using Type = I;
};

template <typename List, template<typename X, typename Y> class F, typename I>
using Accumulate = typename AccumulateT<List, F, I>::Type;

} //_8 --------------------------------------------------------------

namespace _9 {

using namespace _8;

template <typename T, typename U>
class LargeTypeT : public IfThenElse<sizeof(T)>=sizeof(U), T, U>
{};

template <typename List>
class LargestTypeAccT : public AccumulateT<PopFront<List>, LargeTypeT, Front<List>>
{};

} //_9 --------------------------------------------------------------

namespace _10 {

template <typename List, typename Elem, template<typename T, typename U> class Cmp, bool = IsEmpty<List>::value>
class InsertedSortT;

template <typename List, typename Elem, template<typename T, typename U> class Cmp>
class InsertedSortT<List, Elem, Cmp, false> {
    using NewTail = typename IfThenElse<Cmp<Elem, Front<List>>::value,
                                IdentityT<List>,
                                InsertedSortT<PopFront<List>, Elem, Cmp>>::Type;

    using NewHead = typename IfThenElse<Cmp<Elem, Front<List>>::value,
                                Elem,
                                Front<List>>::Type;

public:
    using Type = PushFront<NewTail, NewHead>;
};

template <typename List, typename Elem, template<typename T, typename U> class Cmp>
class InsertedSortT<List, Elem, Cmp, true> : public PushFrontT<List, Elem>
{};

template <typename List, typename Elem, template<typename T, typename U> class Cmp>
using InsertedSort = typename InsertedSortT<List, Elem, Cmp>::Type;

template <typename List, template<typename T, typename U> class Cmp, bool = IsEmpty<List>::value>
class InsertionSortT;

template <typename List, template<typename T, typename U> class Cmp>
using InsertionSort = typename InsertionSortT<List, Cmp>::Type;

template <typename List, template<typename T, typename U> class Cmp>
class InsertionSortT<List, Cmp, false>
    : public InsertedSortT<InsertionSort<PopFront<List>, Cmp>, Front<List>, Cmp>
{};

} //_10 -------------------------------------------------------------

namespace _11 { // compile time value

template <typename T, T Value>
struct CTValue {
    static constexpr T value = Value;
};

template <typename T, T... Values>
using CTValues = TypeList<CTValue<T, Values>...>;

#if (0)
using Primes = TypeList<CTValue<int, 2>,
                        CTValue<int, 3>,
                        CTValue<int, 5>,
                        CTValue<int, 7>,
                        CTValue<int, 11>>;
#else
using Primes = CTValues<int, 2, 3, 5, 7, 11>;
#endif

template <typename T, typename U>
struct MultiplyT;

template <typename T, T Value0, T Value1>
struct MultiplyT<CTValue<T, Value0>, CTValue<T, Value1>> {
public:
    using Type = CTValue<T, Value0*Value1>;
};

template <typename T, typename U>
using Multiply = typename MultiplyT<T, U>::Type;

using namespace _8;

void test()
{
    //TODO : result -> 2 ???
    std::cout << Accumulate<Primes, MultiplyT, CTValue<int, 1>>::value << "\n";
}

} //_11 -------------------------------------------------------------

namespace _12 {

class Null
{};

template <typename H, typename T = Null>
class Cons {
    using Head = H;
    using Tail = T;
};

using Shorts = Cons<short, Cons<unsigned>>;

template <typename List>
class FrontT {
public:
    using Type = typename List::Head;
};

template <typename List>
using Front = typename FrontT<List>::Type;

template <typename List, typename Elem>
class PushFrontT {
public:
    using Type = Cons<Elem, List>;
};

template <typename List, typename Elem>
using PushFront = typename PushFrontT<List, Elem>::Type;

template <typename List>
class PopFrontT {
public:
    using Type = typename List::Tail;
};

template <typename List>
using PopFront = typename PopFrontT<List>::Type;

template <typename List>
struct IsEmpty {
    static constexpr bool value = false;
};

template <>
struct IsEmpty<Null> {
    static constexpr bool value = true;
};

} //_12 -------------------------------------------------------------

} //namespace =================================================================

void test_ch_24()
{
    std::cout << "\n";

    _11::test();
}
