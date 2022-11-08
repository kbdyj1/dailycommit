#ifndef VARIANTCHOICE_HPP
#define VARIANTCHOICE_HPP

#include <utility>
#include "Variant.hpp"
#include "FindIndexOf.hpp"

template <typename T, typename... Types>
class VariantChoice {
    using Derived = Variant<Types...>;

    Derived& derived() { return *static_cast<Derived*>(this); }
    const Derived& derived() const { return *static_cast<const Derived*>(this); }

protected:
    constexpr static unsigned Discriminator = FindIndexOfT<TypeList<Types...>, T>::value + 1;

public:
    VariantChoice()
    {}
    VariantChoice(const T& value) {
        new(derived().rawData()) T(value);
        derived().setDiscriminator(Discriminator);
    }
    VariantChoice(T&& value) {
        new(derived().rawData()) T(std::move(value));
        derived().setDiscriminator(Discriminator);
    }
    bool destroy() {
        if (derived().getDiscriminator() == Discriminator) {
            derived().template as<T>()->~T();
            return true;
        }
        return false;
    }
    Derived& operator=(const T& value) {
        if (derived().getDiscriminator() == Discriminator) {
            *derived().template as<T>() = value;
        } else {
            derived().destroy();
            new(derived().rawData()) T(value);
            derived().setDiscriminator(Discriminator);
        }
        return derived();
    }
    Derived& operator=(T&& value) {
        if (derived().getDiscriminator() == Discriminator) {
            *derived().template as<T>() = std::move(value);
        } else {
            derived().destroy();
            new(derived().rawData()) T(std::move(value));
            derived().setDiscriminator(Discriminator);
        }
        return derived();
    }
};

#endif // VARIANTCHOICE_HPP
