// (C) 2009 christian.schladetsch@gmail.com

#pragma once

/// type traits are used to map information to a C++ type.
template <class T>
struct Traits;

template <class T, int N>
struct TraitsBase
{
    typedef T Type;
    enum { Number = N };
};

#define CGL_TRAITS(T, N) \
    template <> \
    struct Traits<T> : TraitsBase<T, N> { };

