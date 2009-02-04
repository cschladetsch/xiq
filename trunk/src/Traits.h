// (C) 2009 christian.schladetsch@gmail.com

#ifndef TRAITS_H_INCLUDED
#define TRAITS_H_INCLUDED

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

#endif // TRAITS_H_INCLUDED

//EOF

