#ifndef IFTHENELSE_HPP
#define IFTHENELSE_HPP

template <bool Cond, typename TrueType, typename FalseType>
struct IfThenElseT {
    using Type = TrueType;
};

template <typename TrueType, typename FalseType>
struct IfThenElseT<false, TrueType, FalseType> {
    using Type = FalseType;
};

template <bool Cond, typename TrueType, typename FalseType>
using IfThenElse = typename IfThenElseT<Cond, TrueType, FalseType>::Type;

//Identity
template <typename T>
struct IdentityT {
    using Type = T;
};

#endif // IFTHENELSE_HPP
