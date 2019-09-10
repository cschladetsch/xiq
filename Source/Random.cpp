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

Vector2 RandomUnitVector()
{
    return Vector2(RandomUnit() - 0.5f, RandomUnit() - 0.5f).Normalised();
}


