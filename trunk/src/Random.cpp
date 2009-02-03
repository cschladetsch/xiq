// (C) 2009 www.christian.schladetsch.net

#include "Common.h"
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
