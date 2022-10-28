#ifndef ISCONVERTIBLE_HPP
#define ISCONVERTIBLE_HPP

#include <utility>

//template <typename From, typename To, bool = std::is_void_v<To> ||
//                                             std::is_array_v<To> ||
//                                             std::is_function_v<To>>
//struct IsConvertibleHelper {
//    using Type = std::integral_constant<bool, std::is_void_v<To> && std::is_void_v<From>>;
//};

template <typename From, typename To>
struct IsConvertibleHelper {
private:
    static void helper(To);

    template<typename F, typename = decltype(helper(std::declval<F>()))>
    static std::true_type test(void*);

    template<typename>
    static std::false_type test(...);

public:
    using Type = decltype(test<From>(nullptr));
};

template <typename From, typename To>
struct IsConvertibleT : IsConvertibleHelper<From, To>::Type
{};

template <typename From, typename To>
using IsConvertable = IsConvertibleT<From, To>;

template <typename From, typename To>
constexpr bool isConvertible = IsConvertibleT<From, To>::value;

#endif // ISCONVERTIBLE_HPP
