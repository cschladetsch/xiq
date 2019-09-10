// (C) 2009 christian.schladetsch@gmail.com

#pragma once

namespace math
{
    static const float pi = 3.1415926f;
    static const float two_pi = 2.0f*pi;
}

/// A two-dimensional vector (also used for points)
struct Vector2
{
    typedef float Ordinate;

    Ordinate x, y;

    Vector2() : x(0), y(0) { }
    Vector2(Ordinate _x, Ordinate _y) : x(_x), y(_y) { }

    friend bool operator==(Vector2 const &A, Vector2 const &B)
    {
        return A.x == B.x && A.y == B.y;
    }

    Vector2 &operator*=(float F)
    {
        x *= F;
        y *= F;
        return *this;
    }

    Vector2 &operator+=(Vector2 const &V)
    {
        x += V.x;
        y += V.y;
        return *this;
    }

    Vector2 &operator-=(Vector2 const &V)
    {
        x -= V.x;
        y -= V.y;
        return *this;
    }

    Vector2 Normalised() const
    {
        float mag = Length();
        if (mag == 0) return Vector2(1,0);
        return Vector2(x/mag, y/mag);
    }

    void Normalise()
    {
        *this = Normalised();
    }

    float LengthSquared() const { return x*x + y*y; }
    float Length() const { return sqrt(LengthSquared()); }

    Vector2 Normal() const
    {
        return Vector2(-y, x).Normalised();
    }

    Vector2 OtherNormal() const
    {
        return Vector2(y, -x).Normalised();
    }
};

typedef Vector2 Point2;

inline Vector2 operator*(Vector2 const &V, float F)
{
    return Vector2(V) *= F;
}

inline Vector2 operator+(Vector2 const &A, Vector2 const &B)
{
    return Vector2(A) += B;
}

inline Vector2 operator-(Vector2 const &A, Vector2 const &B)
{
    return Vector2(A) -= B;
}

inline float DotProduct(Vector2 const &A, Vector2 const &B)
{
    return A.x*B.x + A.y*B.y;
}

inline float Angle(Vector2 const &A, Vector2 const &B)
{
    return acos(DotProduct(A.Normalised(),B.Normalised()));
}

/// a 3D vector, also a homogenous 2D point
struct Vector3
{
    typedef float Ordinate;
    Ordinate x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(Ordinate _x, Ordinate _y, Ordinate _z) : x(_x), y(_y), z(_z) { }
};

inline float DotProduct(Vector3 const &A, Vector3 const &B)
{
    return A.x*B.x + A.y*B.y + A.z*B.z;
}

#include "Matrix.h"

/// A line-segment has a start and end point.
struct LineSegment
{
    Point2 first, second;

    LineSegment() { }
    LineSegment(Point2 const &F, Point2 const &S) : first(F), second(S) { }

    /// return the vector of the line segment
    Vector2 GetVector() const { return second - first; }

    /// return a point point on the linesegment at a given parameterised time
    Point2 AtTime(float T) const { return first + GetVector()*T; }

    /// return a normal to the line
    Vector2 Normal() const
    {
        return GetVector().Normal();
    }

    /// return the other normal to the line
    Vector2 OtherNormal() const
    {
        return GetVector().OtherNormal();
    }
};

/// reflect a vector from a line-segment
inline Vector2 Reflect(Vector2 const &V, LineSegment const &L)
{
    Vector2 N = L.Normal();
    return V - N*2.0f*DotProduct(N, V);
}

/// an indefinite-length line
struct Line
{
    float a, b, c;    // ax + by = c

    Line() : a(0), b(0), c(0)
    {
    }

    Line(LineSegment const &segment)
    {
        CreateFrom(segment);
    }

    Line(Point2 A, Point2 B)
    {
        CreateFrom(LineSegment(A,B));
    }

    void CreateFrom(LineSegment const &segment)
    {
        float x1 = segment.first.x;
        float y1 = segment.first.y;
        float x2 = segment.second.x;
        float y2 = segment.second.y;
        a = y2 - y1;
        b = x1 - x2;
        c = a*x1 + b*y1;
    }

    /// return the intersection of two lines, or Point() if they are parallel
    static Point2 Intersection(Line const &L1, Line const &L2)
    {
        float det = Determinant(L1, L2);
        if (det == 0)
        {
            return Point2(); //Lines are parallel
        }
        float A1 = L1.a, B1 = L1.b, C1 = L1.c;
        float A2 = L2.a, B2 = L2.b, C2 = L2.c;
        float x = (B2*C1 - B1*C2)/det;
        float y = (A1*C2 - A2*C1)/det;
        return Point2(x, y);
    }

    static float Determinant(Line const &L1, Line const &L2)
    {
        float A1 = L1.a, B1 = L1.b;
        float A2 = L2.a, B2 = L2.b;
        return A1*B2 - A2*B1;
    }

    /// returns true if the two lines intersects
    static bool Intersects(Line const &L1, Line const &L2)
    {
        return Determinant(L1, L2) != 0;
    }

    Point2 Intersection(Line const &other) const
    {
        return Intersection(*this, other);
    }

    bool Intersects(Line const &other) const
    {
        return Intersects(*this, other);
    }
};

/// an axis-aligned rectangle
struct Box
{
    Point2 top_left;
    Point2 top_right;
    Point2 bottom_left;
    Point2 bottom_right;

    Box() { }
    Box(Point2 TL, Point2 BR)
    {
        top_left = TL;
        bottom_right = BR;
        top_right = Point2(BR.x, TL.y);
        bottom_left = Point2(TL.x, BR.y);
    }
};

void DistanceFromLine(Point2 C, Vector2 A, Vector2 B, float &distance_segment, float &distance_line);

inline void DistanceFromLine(Point2 C, LineSegment line, float &distance_segment, float &distance_line)
{
    DistanceFromLine(C, line.first, line.second, distance_segment, distance_line);
}

/// return the nearest point of a given point to a line segment
Point2 NearestPoint(LineSegment const &L, Point2 const &p3);

inline LineSegment Transform(LineSegment const &ls, Matrix const &matrix)
{
    return LineSegment(matrix*ls.first, matrix*ls.second);
}

