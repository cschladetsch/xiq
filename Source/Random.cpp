// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "Random.h"

float RandomUnit()
{
	return (float)rand()/RAND_MAX;
}

float RandomRanged(float min, float max)
{
	return min + (max - min)*RandomUnit();
}

Vector RandomUnitVector()
{
	return Vector(RandomUnit() - 0.5f, RandomUnit() - 0.5f).Normalised();
}

//EOF
