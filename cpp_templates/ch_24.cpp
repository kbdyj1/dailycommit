//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.24 Type list
//
//  22/11/03
//=============================================================================

namespace { //=================================================================

template <typename... Types>
class TypeList
{};

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

} //namespace =================================================================

void test_ch_24()
{

}
