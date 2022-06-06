#include <iostream>

namespace { //=================================================================

// Address of
class NonAddressable
{
public:
    typedef double uselessType;
private:
    uselessType operator&() const;
};

template <typename T>
T* addressOf(T& v) noexcept
{
    return reinterpret_cast<T*>(
                &const_cast<char&>(
                    reinterpret_cast<const volatile char&>(v)));
}

// Algebraic Hierarchy

struct BaseConstructor
{
    BaseConstructor(int = 0)
    {}
};

class Real;
class Complex;
class Number;

class Number
{
    friend class Real;
    friend class Complex;
public:
    Number& operator=(const Number& n);
    Number(const Number&n);
    virtual ~Number();

    virtual Number operator+(Number const& n) const;
    void swap(Number& n) noexcept;

    static Number makeReal(double r);
    static Number makeComplex(double r, double i);

protected:
    Number();
    Number(BaseConstructor);

private:
    void redefine(Number* n);
    virtual Number complexAdd(Complex const& n) const;
    virtual Number realAdd(Real const& n) const;

    Number* rep;
    short refCount;
};

class Complex : public Number
{
    friend class Real;
    friend class Number;

    Complex(double r, double i);
    Complex(const Complex&);
    virtual ~Complex();

    virtual Number operator+(Number const& n) const;
    virtual Number complexAdd(Complex const& n) const;
    virtual Number realAdd(Real const& n) const;

    double rPart;
    double iPart;
};

class Real : public Number
{
    friend class Complex;
    friend class Number;

    Real(double r);
    Real(const Real& r);
    virtual ~Real();

    virtual Number operator+(Number const& n) const;
    virtual Number complexAdd(Complex const& n) const;
    virtual Number realAdd(Real const& n) const;

    double value;
};

Number::Number(BaseConstructor)
    : rep(nullptr)
    , refCount(0)
{}

Number::Number()
    : rep(nullptr)
    , refCount(0)
{}

Number::Number(const Number& n)
    : rep(n.rep)
    , refCount(0)
{
    if (rep)
        rep->refCount++;
}

Number Number::makeReal(double r)
{
    Number n;
    n.redefine(new Real(r));
    return n;
}

Number Number::makeComplex(double r, double i)
{
    Number n;
    n.redefine(new Complex(r, i));
    return n;
}

Number::~Number()
{
    if (rep && --rep->refCount == 0)
        delete rep;
}

Number& Number::operator=(const Number &n)
{
    Number temp(n);
    this->swap(temp);
    return *this;
}

void Number::swap(Number &n) noexcept
{
    std::swap(this->rep, rep);
}

Number Number::operator+(const Number &n) const
{
    return rep->operator+(n);
}

Number Number::complexAdd(const Complex &n) const
{
    return rep->complexAdd(n);
}

Number Number::realAdd(const Real &n) const
{
    return rep->realAdd(n);
}

void Number::redefine(Number *n)
{
    if (rep && --rep->refCount)
        delete rep;
    rep = n;
}

Complex::Complex(double d, double e)
    : Number(BaseConstructor{})
    , rPart(d)
    , iPart(e)
{}

Complex::Complex(const Complex& c)
    : Number(BaseConstructor{})
    , rPart(c.rPart)
    , iPart(c.iPart)
{}

Complex::~Complex()
{}

Number Complex::operator+(const Number &n) const
{
    return n.complexAdd(*this);
}

Number Complex::realAdd(const Real &n) const
{
    return Number::makeComplex(this->rPart + n.value, this->iPart);
}

Number Complex::complexAdd(const Complex &n) const
{
    return Number::makeComplex(this->rPart+n.rPart, this->iPart+n.iPart);
}

Real::Real(double r)
    : Number(BaseConstructor{})
    , value(r)
{}

Real::Real(const Real& r)
    : Number(BaseConstructor{})
    , value(r.value)
{}

Real::~Real()
{}

Number Real::operator+(const Number &n) const
{
    return n.realAdd(*this);
}

Number Real::realAdd(const Real &n) const
{
    return Number::makeReal(this->value + n.value);
}

Number Real::complexAdd(const Complex &n) const
{
    return Number::makeComplex(n.rPart + this->value, n.iPart);
}

} //namespace =================================================================
void test_addressof()
{
    NonAddressable na;
#if (0)
    NonAddressable* ptr = &na;
#else
    NonAddressable* ptr = addressOf(na);
    NonAddressable* ptr2 = std::addressof(na);

    std::cout << "ptr: " << ptr << "\n";
    std::cout << "ptr2: " << ptr2 << "\n";
#endif
}

void test_algebraic_hierarchy()
{
    Number c = Number::makeComplex(1, 2);
    Number r = Number::makeReal(10);
    Number result = c + r;
}
