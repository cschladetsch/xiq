// (C) 2009 www.christian.schladetsch.net

#ifndef CLAMP_H_INCLUDED
#define CLAMP_H_INCLUDED

/// returns a value clamped between a min and max limit
template <class Scalar>
Scalar Clamp(Scalar val, Scalar min, Scalar max)
{
	return std::max(std::min(max, val), min);
}

#endif // CLAMP_H_INCLUDED

//EOF
