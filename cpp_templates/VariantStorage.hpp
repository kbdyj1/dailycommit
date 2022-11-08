#ifndef VARIANTSTORAGE_H
#define VARIANTSTORAGE_H

#include <new>
#include "TypeList.hpp"

template <typename... Types>
class VariantStorage {
    using LargestT = LargestType<TypeList<Types...>>;
    alignas(Types...) unsigned char buffer[sizeof(LargestT)];
    unsigned char discriminator = 0;

public:
    unsigned char getDiscriminator() const {
        return discriminator;
    }
    void setDiscriminator(unsigned char d) {
        discriminator = d;
    }

    void* rawData() { return buffer; }
    const void* rawData() const { return buffer; }

    template<typename T>
    T* as() { return std::launder(reinterpret_cast<T*>(buffer)); }
    template<typename T>
    const T* as() const { return std::launder(reinterpret_cast<const T*>(buffer)); }
};

#endif // VARIANTSTORAGE_H
