//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.25 Tuple
//
//  22/11/06
//=============================================================================

#include <iostream>
#include "TypeList.hpp"
#include "ValueList.hpp"
#include "Tuple.hpp"

template <>
struct IsEmpty<Tuple<>> {
    static constexpr bool value = true;
};

template <typename Head, typename... Tail>
class FrontT<Tuple<Head, Tail...>> {
public:
    using Type = Head;
};

template <typename Head, typename... Tail>
class PopFrontT<Tuple<Head, Tail...>> {
public:
    using Type = Tuple<Tail...>;
};

template <typename... List, typename New>
class PushFrontT<Tuple<List...>, New> {
public:
    using Type = Tuple<New, List...>;
};

template <typename... List, typename New>
class PushBackT<Tuple<List...>, New> {
public:
    using Type = Tuple<List..., New>;
};

void print(std::ostream& os, const Tuple<>&, bool isFirst = true)
{
    os << (isFirst ?  '(' : ')');
}

template <typename Head, typename... Tail>
void print(std::ostream& os, const Tuple<Head, Tail...>& t, bool isFirst = true)
{
    os << (isFirst ? "(" : ", ");
    os << t.getHead();
    print(os, t.getTail(), false);
}

template <typename... Types>
std::ostream& operator<<(std::ostream& os, const Tuple<Types...>& t)
{
    print(os, t);
    return os;
}

#if (0) //TODO : Fix compile error
template <typename... Types, typename Value>
PushFront<Tuple<Types...>, Value>
pushFront(const Tuple<Types...>& tuple, const Value& value)
{
    return PushFront<Tuple<Types...>, Value>(value, tuple);
}
#endif

template <typename V>
Tuple<V> pushBack(const Tuple<>&, const V& value)
{
    return Tuple<V>(value);
}

template <typename Head, typename... Tail, typename V>
Tuple<Head, Tail..., V>
pushBack(const Tuple<Head, Tail...>& t, V value)
{
    return Tuple<Head, Tail..., V>(t.getHead(), pushBack(t.getTail(), value));
}

template <typename... Types>
typename PopFrontT<Tuple<Types...>>::Type
popFront(const Tuple<Types...>& tuple)
{
    return tuple.getTail();
}

namespace { //=================================================================

namespace _1 {

void test()
{
    Tuple<int, double, std::string> t(1, 3.4, "Qt6.4");
    Tuple<int, double, std::string> c(t);

    std::cout << get<0>(t) << "\n";
    std::cout << get<1>(t) << "\n";
    std::cout << get<2>(t) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

bool operator==(const Tuple<>&, const Tuple<>&)
{
    return true;
}

template <typename Head0, typename... Tail0,
          typename Head1, typename... Tail1,
          typename = EnableIf<sizeof...(Tail0)==sizeof...(Tail1)>>
bool operator==(const Tuple<Head0, Tail0...>& l, const Tuple<Head1, Tail1...>& r)
{
    return l.getHead() == r.getHead() && l.getTail() == r.getTail();
}

void test()
{
    Tuple<int, double, std::string> t(1, 3.4, "Qt6.4");
    Tuple<int, double, std::string> c(t);
    Tuple<int, double, std::string> d(1, 3.3, "Qt6.4");

    std::cout << "(t == c): " << (t == c) << "\n";
    std::cout << "(t == d): " << (t == d) << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    auto t = makeTuple(1, 3.4, "Qt6.0");
    std::cout << t;
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
    auto t0 = makeTuple(1, 3.4, "Qt6.0");

    using T1 = PopFrontT<PushBackT<decltype(t0), bool>::Type>::Type;

    T1 t1(get<1>(t0), get<2>(t0), true);

    std::cout << t1 << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <int N>
struct CopyCounter
{
    inline static unsigned copyCnt = 0;
    CopyCounter()
    {}
    CopyCounter(const CopyCounter&)
    {
        ++copyCnt;
    }
};

Tuple<> reverse(const Tuple<>& t)
{
    return t;
}

template <typename Head, typename... Tail>
Reverse<Tuple<Head, Tail...>> reverse(const Tuple<Head, Tail...>& t)
{
    return pushBack(reverse(t.getTail()), t.getHead());
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <unsigned N, typename Result = ValueList<unsigned>>
struct MakeIndexListT : MakeIndexListT<N-1, PushFront<Result, CTValue<unsigned, N-1>>>
{
};

template <typename Result>
struct MakeIndexListT<0, Result>
{
    using Type = Result;
};

template <unsigned N>
using MakeIndexList = typename MakeIndexListT<N>::Type;

template <typename... Elements, unsigned... Indices>
auto reverseImpl(const Tuple<Elements...>& t, ValueList<unsigned, Indices...>&)
{
    return makeTuple(get<Indices>(t)...);
}

template <typename... Elements>
auto reverse(const Tuple<Elements...>& t)
{
    return reverseImpl(t, Reverse<MakeIndexList<sizeof...(Elements)>>());
}

} //_6 --------------------------------------------------------------

namespace _7 {

template <typename... Elements, unsigned... Indices>
auto select(const Tuple<Elements...>& t, ValueList<unsigned, Indices...>)
{
    return makeTuple(get<Indices>(t)...);
}

template <unsigned I, unsigned N, typename IndexList = ValueList<unsigned>>
class ReplicatedIndexListT;

template <unsigned I, unsigned N, unsigned... Indices>
class ReplicatedIndexListT<I, N, ValueList<unsigned, Indices...>>
    : public ReplicatedIndexListT<I, N-1, ValueList<unsigned, Indices..., I>>
{};

template <unsigned I, unsigned... Indices>
class ReplicatedIndexListT<I, 0, ValueList<unsigned, Indices...>> {
public:
    using Type = ValueList<unsigned, Indices...>;
};

template <unsigned I, unsigned N>
using ReplicatedIndexList = typename ReplicatedIndexListT<I, N>::Type;

template <unsigned I, unsigned N, typename... Elements>
auto splat(const Tuple<Elements...>& t)
{
    return select(t, ReplicatedIndexList<I, N>());
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_25()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
