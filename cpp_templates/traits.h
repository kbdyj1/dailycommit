#ifndef TRAITS_H
#define TRAITS_H

template <bool, typename T=void>
struct EnableIfT {
};

template <typename T>
struct EnableIfT<true, T> {
    using Type = T;
};

template <bool Cond, typename T = void>
using EnableIf = typename EnableIfT<Cond, T>::Type;

#endif // TRAITS_H
