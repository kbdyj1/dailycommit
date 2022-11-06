#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <utility>
#include "EnableIf.hpp"

template <typename... Types>
class Tuple;

template <typename Head, typename... Tail>
class Tuple<Head, Tail...>
{
    Head head;
    Tuple<Tail...> tail;

public:
    Tuple()
    {}

    Tuple(const Head& head, const Tail&... tail) : head(head), tail(tail...)
    {
//        std::cout << "Tuple(const Head&, const Tail&...(" << sizeof...(tail) << ")\n";
    }
    template<typename VHead, typename... VTail, typename = EnableIf<sizeof...(VTail) == sizeof...(Tail)>>
    Tuple(VHead&& head, VTail&&... tail) : head(std::forward<VHead>(head)), tail(std::forward<VTail>(tail)...)
    {
//        std::cout << "Tuple(Head&&, Tail&&...(" << sizeof...(tail) << ")\n";
    }
    template<typename VHead, typename... VTail, typename = EnableIf<sizeof...(VTail) == sizeof...(Tail)>>
    Tuple(const Tuple<VHead, VTail...>& other) : head(other.getHead()), tail(other.getTail())
    {}

    Head& getHead() { return head; }
    const Head& getHead() const { return head; }
    Tuple<Tail...>& getTail() { return tail; }
    const Tuple<Tail...>& getTail() const { return tail; }
};

template <>
class Tuple<>
{};

template <unsigned N>
struct Get {
    template<typename Head, typename... Tail>
    static auto apply(const Tuple<Head, Tail...>& t) {
        return Get<N-1>::apply(t.getTail());
    }
};

template<>
struct Get<0> {
    template<typename Head, typename... Tail>
    static const Head& apply(const Tuple<Head, Tail...>& t) {
        return t.getHead();
    }
};

template <unsigned N, typename... Types>
auto get(const Tuple<Types...>& t)
{
    return Get<N>::apply(t);
}

template <typename... Types>
auto makeTuple(Types&&... elems)
{
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(elems)...);
}

#endif // TUPLE_HPP
