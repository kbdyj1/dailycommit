#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <exception>
#include "VariantStorage.hpp"
#include "VariantChoice.hpp"

class EmptyVariant : public std::exception
{};

template <typename... Types>
class Variant
    : private VariantStorage<Types...>
    , private VariantChoice<Types, Types...>...
{
    template<typename T, typename... OtherTypes>
    friend class VariantChoice;

public:
    template<typename T>
    bool is() const {
        return this->getDiscriminator() == VariantChoice<T, Types...>::Discriminator;
    }

    template<typename T>
    T& get()& {
        if (empty()) {
            throw EmptyVariant();
        }
        assert(is<T>());
        return *this->template as<T>();
    }

    template<typename T>
    const T& get() const& {
        if (empty()) {
            throw EmptyVariant();
        }
        assert(is<T>());
        return *this->template as<T>();
    }

    template<typename T>
    T&& get()&&;

#if (0)
    template<typename R = ComputedResultType, typename Visitor>
    VisitResult<R, Visitor, Types&...> visit(Visitor&& visitor)&;
    template<typename R = ComputedResultType, typename Visitor>
    VisitResult<R, Visitor, const Types&...> visit(Visitor&& visitor) const&;
    template<typename R = ComputedResultType, typename Visitor>
    VisitResult<R, Visitor, Types&&...> visit(Visitor&& visitor)&&;
#endif

    using VariantChoice<Types, Types...>::VariantChoice...;

    Variant();

    Variant(const Variant& other);

    Variant(Variant&& other);

    template<typename... SourceTypes>
    Variant(const Variant<SourceTypes...>&);

    template<typename... SourceTypes>
    Variant(Variant<SourceTypes...>&&);

    using VariantChoice<Types, Types...>::operator=...;

    Variant& operator=(const Variant& other);

    Variant& operator=(Variant&& other);

    template<typename... SourceTypes>
    Variant& operator=(const Variant<SourceTypes...>&);

    template<typename... SourceTypes>
    Variant& operator=(Variant<SourceTypes...>&&);

    bool empty() const;

    ~Variant() { destroy(); }

    void destroy() {
        bool results[] = {
            VariantChoice<Types, Types...>::destroy()...
        };
        this->setDiscriminator(0);
    }
};

#endif // VARIANT_HPP
