//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.24 Type list
//
//  22/11/03
//=============================================================================

#include "IfThenElse.hpp"

namespace { //=================================================================

template <typename... Types>
class TypeList
{};

template <typename List>
class IsEmpty {
public:
    static constexpr bool value = false;
};

template <>
class IsEmpty<TypeList<>> {
public:
    static constexpr bool value = true;
};

using SignedIntegralTypes = TypeList<signed char, short, int, long, long long>;

template <typename List>
class FrontT;

template <typename Head, typename... Tail>
class FrontT<TypeList<Head, Tail...>> {
public:
    using Type = Head;
};

template <typename List>
using Front = typename FrontT<List>::Type;

template <typename List>
class PopFrontT;

template <typename Head, typename... Tail>
class PopFrontT<TypeList<Head, Tail...>> {
public:
    using Type = TypeList<Tail...>;
};

template <typename Head, typename... Tail>
using PopFront = typename PopFrontT<TypeList<Head, Tail...>>::Type;

template <typename List, typename NewType>
class PushFrontT;

template <typename... Types, typename NewType>
class PushFrontT<TypeList<Types...>, NewType> {
public:
    using Type = TypeList<NewType, Types...>;
};

template <typename List, typename NewType>
using PushFront = typename PushFrontT<List, NewType>::Type;

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

template <typename List, typename New>
class PushBackT;

template <typename... Lists, typename New>
class PushBackT<TypeList<Lists...>, New> {
public:
    using Type = TypeList<Lists..., New>;
};

template <typename Lists, typename New>
using PushBack = typename PushBackT<Lists, New>::Type;

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

} //namespace =================================================================

void test_ch_24()
{

}
