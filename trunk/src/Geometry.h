// (C) 2009 www.christian.schladetsch.net

#ifndef GEOMETRY_H_INCLUDED
#	define GEOMETRY_H_INCLUDED

namespace math
{
	static const float pi = 3.1415926f;
	static const float two_pi = 2.0f*pi;
}

/// A two-dimensional vector (also used for points)
struct Vector
{
	typedef float Ordinate;

	Ordinate x, y;

	Vector() : x(0), y(0) { }
	Vector(Ordinate _x, Ordinate _y) : x(_x), y(_y) { }

	friend bool operator==(Vector const &A, Vector const &B)
	{
		return A.x == B.x && A.y == B.y;
	}

	Vector &operator*=(float F)
	{
		x *= F;
		y *= F;
		return *this;
	}

	Vector &operator+=(Vector const &V)
	{
		x += V.x;
		y += V.y;
		return *this;
	}

	Vector &operator-=(Vector const &V)
	{
		x -= V.x;
		y -= V.y;
		return *this;
	}

	Vector Normalised() const
	{
		float mag = Length();
		if (mag == 0) return Vector(1,0);
		return Vector(x/mag, y/mag);
	}

	void Normalise()
	{
		*this = Normalised();
	}

	float LengthSquared() const { return x*x + y*y; }
	float Length() const { return sqrt(LengthSquared()); }

	Vector Normal() const
	{
		return Vector(-y, x).Normalised();
	}

	Vector OtherNormal() const
	{
		return Vector(y, -x).Normalised();
	}
};

// TODO: should be typedef Vector2 Vector; but Vector came before i added Vector3...
typedef Vector Vector2;

inline Vector operator*(Vector const &V, float F)
{
	return Vector(V) *= F;
}

inline Vector operator+(Vector const &A, Vector const &B)
{
	return Vector(A) += B;
}

inline Vector operator-(Vector const &A, Vector const &B)
{
	return Vector(A) -= B;
}

inline float DotProduct(Vector const &A, Vector const &B)
{
	return A.x*B.x + A.y*B.y;
}

inline float Angle(Vector const &A, Vector const &B)
{
	return acos(DotProduct(A.Normalised(),B.Normalised()));
}

typedef Vector Point;

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

//struct LineType
//{
//	enum Type
//	{
//		None, Line, NewLine, Edge
//	};
//	Type type;
//	LineType(Type T = None) : type(T) { }
//
//	bool operator==(Type T) { return T == type; }
//};

/// A line-segment has a start and end point.
struct LineSegment
{
	Point first, second;

	LineSegment() { }
	LineSegment(Point const &F, Point const &S) : first(F), second(S) { }

	/// return the vector of the line segment
	Vector GetVector() const { return second - first; }

	/// return a point point on the linesegment at a given parameterised time
	Point AtTime(float T) const { return first + GetVector()*T; }

	/// return a normal to the line
	Vector Normal() const
	{
		return GetVector().Normal();
	}

	/// return the other normal to the line
	Vector OtherNormal() const
	{
		return GetVector().OtherNormal();
	}
};

/// reflect a vector from a line-segment
inline Vector Reflect(Vector const &V, LineSegment const &L)
{
	Vector N = L.Normal();
	return V - N*2.0f*DotProduct(N, V);
}

/// an indefinite-length line
struct Line
{
	float a, b, c;	// ax + by = c

	Line() : a(0), b(0), c(0)
	{
	}
	Line(LineSegment const &segment)
	{
		CreateFrom(segment);
	}
	Line(Point A, Point B)
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
	static Point Intersection(Line const &L1, Line const &L2)
	{
		float det = Determinant(L1, L2);
		if (det == 0)
		{
			return Point(); //Lines are parallel
		}
		float A1 = L1.a, B1 = L1.b, C1 = L1.c;
		float A2 = L2.a, B2 = L2.b, C2 = L2.c;
		float x = (B2*C1 - B1*C2)/det;
		float y = (A1*C2 - A2*C1)/det;
		return Point(x, y);
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

	Point Intersection(Line const &other) const
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
	Point top_left;
	Point top_right;
	Point bottom_left;
	Point bottom_right;
};

void DistanceFromLine(Point C, Vector A, Vector B, float &distance_segment, float &distance_line);

inline void DistanceFromLine(Point C, LineSegment line, float &distance_segment, float &distance_line)
{
	DistanceFromLine(C, line.first, line.second, distance_segment, distance_line);
}

/// return the nearest point of a given point to a line segment
Point NearestPoint(LineSegment const &L, Point const &p3);

#endif // GEOMETRY_H_INCLUDED

//EOF
