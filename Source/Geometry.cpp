// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"

Point2 NearestPoint(LineSegment const &L, Point2 const &p3)
{
    Point2 p1 = L.first;
    Point2 p2 = L.second;
    float ls = (p2 - p1).LengthSquared();
    if (ls == 0)
    {
        return p1;
    }

    float x1 = p1.x, x2 = p2.x, x3 = p3.x;
    float y1 = p1.y, y2 = p2.y, y3 = p3.y;
    float u = ((x3 - x1)*(x2 - x1) + (y3 - y1)*(y2 - y1))/ls;

    if (u > 1)
        return L.second;
    if (u < 0)
        return L.first;

    float x = x1 + u*(x2 - x1);
    float y = y1 + u*(y2 - y1);

    return Point2(x, y);
}

void DistanceFromLine(Point2 C, Vector2 A, Vector2 B, float &distanceSegment, float &distanceLine)
{
    float cx = C.x, cy = C.y, ax = A.x, ay = A.y, bx = B.x, by = B.y;
    float r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
    float r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
    float r = r_numerator / r_denomenator;

    float px = ax + r*(bx-ax);
    float py = ay + r*(by-ay);

    float s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;

    distanceLine = fabs(s)*sqrt(r_denomenator);

    float xx = px;
    float yy = py;

    if ( (r >= 0) && (r <= 1) )
    {
        distanceSegment = distanceLine;
    }
    else
    {
        float dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
        float dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
        if (dist1 < dist2)
        {
            xx = ax;
            yy = ay;
            distanceSegment = sqrt(dist1);
        }
        else
        {
            xx = bx;
            yy = by;
            distanceSegment = sqrt(dist2);
        }
    }

    return;
}
