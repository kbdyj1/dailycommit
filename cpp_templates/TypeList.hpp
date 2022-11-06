#ifndef TYPELIST_HPP
#define TYPELIST_HPP

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

template <typename List, typename New>
class PushBackT;

template <typename... Lists, typename New>
class PushBackT<TypeList<Lists...>, New> {
public:
    using Type = TypeList<Lists..., New>;
};

template <typename Lists, typename New>
using PushBack = typename PushBackT<Lists, New>::Type;

#endif // TYPELIST_HPP
