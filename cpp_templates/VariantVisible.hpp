#ifndef VARIANTVISIBLE_HPP
#define VARIANTVISIBLE_HPP

#include <utility>
#include "TypeList.hpp"
#include "Variant.hpp"

template <typename R, typename V, typename Visitor, typename Head, typename... Tail>
R variantVisibleImpl(V&& variant, Visitor&& visitor, TypeList<Head, Tail...>) {
    if (variant.template is<Head>()) {
        return static_cast<R>(std::forward<Visitor>(visitor)(
            std::forward<V>(variant).template get<Head>()
        ));
    } else if constexpr(0 < sizeof...(Tail)) {
        return variantVisibleImpl<R>(std::forward<V>(variant), std::forward<Visitor>(visitor), TypeList<Tail...>());
    } else {
        throw EmptyVariant();
    }
}

#endif
