#ifndef FINDINDEXOF_HPP
#define FINDINDEXOF_HPP

#include "TypeList.hpp"
#include "type_traits"

template <typename List, typename T, unsigned N = 0, bool Empty = IsEmpty<List>::value>
struct FindIndexOfT;

template <typename List, typename T, unsigned N>
struct FindIndexOfT<List, T, N, false> : public IfThenElse<std::is_same<Front<List>, T>::value,
    std::integral_constant<unsigned, N>,
    FindIndexOfT<PopFront<List>, T, N+1>>
{};

template <typename List, typename T, unsigned N>
struct FindIndexOfT<List, T, N, true>
{};

#endif // FINDINDEXOF_HPP
