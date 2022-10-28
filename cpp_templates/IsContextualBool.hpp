#ifndef ISCONTEXTUALBOOL_HPP
#define ISCONTEXTUALBOOL_HPP

#include <utility>

template <typename T>
class IsContextualBoolT {
    template<typename U>
    struct Identity;

    template<typename U>
    static std::true_type test(Identity<decltype(std::declval<U>() ? 0 : 1)>*);

    template<typename U>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template <typename T>
constexpr bool IsContextualBool = IsContextualBoolT<T>::value;

#endif // ISCONTEXTUALBOOL_HPP
