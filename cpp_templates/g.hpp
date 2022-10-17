#ifndef G_HPP
#define G_HPP

namespace g {

template <typename T>
int func(T, T x = 0)
{
    return x;
}

template <>
int func(int, int);

}

#endif // G_HPP
