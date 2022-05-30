#include <iostream>

namespace { //=================================================================

// T.80 Do not naively templatize a class hierarchy

namespace t80 {
namespace bad {

template <typename T>
struct Container {
    virtual T* get(int i);
    virtual T* first();
    virtual T* next();
    virtual void sort();
};

template <typename T>
class Vector : public Container<T> {
public:
};

} //bad
} //t80

// T.81 Do not mix hierarchies and arrays

// T.82 Linearize a hierarchy when virtual functions are undesirable

// T.83 Do not declare a member function template virtual

#if (0)
class Shape {
//  34:5: error: templates may not be ‘virtual’
//   34 |     virtual bool intersect(T* p);
//      |     ^~~~~~~
    template <typename T>
    virtual bool intersect(T* p);
};
#endif

// T.84 Use a non-template core implementation to provide an ABI-stable interface

struct LinkBase {
    LinkBase* suc;
    LinkBase* pre;
};

template <typename T>
struct Link : LinkBase {
    T value;
};

struct ListBase {
    LinkBase* first;
    int size;
    void addFront(LinkBase*);
};

template <typename T>
class List : ListBase {
    void putFront(const T& e) {
        addFront(new Link<T>{e});
    }
    T& front() { return static_cast<Link<T>*>(first)->value; }
};

} //===========================================================================
