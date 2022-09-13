#include <cstddef>

module;

export module Vector;

export class Vector {
    double* elem;
    size_t sz;
public:
    Vector(size_t size);
    double& operator[](int i);
    size_t size() const;
};

//=============================================================================
//  implementation
//=============================================================================

Vector::Vector(size_t size) : sz(size), elem(new double[size])
{}

double& Vector::operator[](const int index)
{
    return elem[index];
}

size_t Vector::size() const
{
    return sz;
}

export size_t size(const Vector& v) {
    return v.size();
}
