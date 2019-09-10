// (C) 2009 christian.schladetsch@gmail.com

#pragma once

/// returns a value clamped between a min and max limit
template <class Scalar>
Scalar Clamp(Scalar val, Scalar min, Scalar max)
{
    return std::max(std::min(max, val), min);
}
