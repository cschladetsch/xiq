// (C) 2009 www.christian.schladetsch.net

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

/// @brief 2D affine transform.
///
/// see http://en.wikipedia.org/wiki/Transformation_matrix
/// and http://www.facstaff.bucknell.edu/mastascu/eLessonsHTML/Circuit/MatVecMultiply.htm
struct Matrix
{
	typedef float Ordinate;

	struct V3 { Ordinate x, y, z; };
	union
	{
		Ordinate m[3][3];
		Ordinate _[9];
		struct
		{
			V3 X;
			V3 Y;
			V3 Z;
		};
	};

	Matrix()
	{
		std::fill(_, _ + 9, 0);
		m[0][0] = m[1][1] = m[2][2] = 1;
	}
	Matrix(Ordinate _0, Ordinate _1, Ordinate _2
		, Ordinate _3, Ordinate _4, Ordinate _5
		, Ordinate _6, Ordinate _7, Ordinate _8)
	{
		_[0] = _0; _[1] = _1; _[2] = _2;
		_[3] = _3; _[4] = _4; _[5] = _5;
		_[6] = _6; _[7] = _7; _[8] = _8;
	}

	Ordinate &operator()(int a, int b)
	{
		return m[a][b];
	}
	Ordinate operator()(int a, int b) const
	{
		return m[a][b];
	}
	Ordinate operator[](int N) const { return _[N]; }
	Ordinate &operator[](int N) { return _[N]; }

	/// make a counter-clockwise rotation matrix
	static Matrix Rotation(float angle)
	{
		float s = sin(angle);
		float c = cos(angle);
		return Matrix
		(
			c, -s, 0,
			s, c, 0,
			0, 0, 1
		);
	}

	static Matrix Scale(Point const &P) { return Scale(P.x, P.y); }
	static Matrix Scale(float sx, float sy)
	{
		return Matrix
		(
			sx, 0, 0,
			0, sy, 0,
			0, 0, 1
		);
	}

	static Matrix Translate(Point const &P) { return Translate(P.x, P.y); }
	static Matrix Translate(float tx, float ty)
	{
		return Matrix
		(
			1, 0, tx,
			0, 1, ty,
			0, 0, 1
		);
	}

	static Matrix Identity()
	{
		return Matrix();
	}

	Matrix Multiply(Matrix const &A) const
	{
		Matrix C;
		Matrix const &B = *this;
		C[0] = A[0]*B[0] + A[1]*B[3] + A[2]*B[6];
		C[1] = A[0]*B[1] + A[1]*B[4] + A[2]*B[7];
		C[2] = A[0]*B[2] + A[1]*B[5] + A[2]*B[8];

		C[3] = A[3]*B[0] + A[4]*B[3] + A[5]*B[6];
		C[4] = A[3]*B[1] + A[4]*B[4] + A[5]*B[7];
		C[5] = A[3]*B[2] + A[4]*B[5] + A[5]*B[8];

		C[6] = A[6]*B[0] + A[7]*B[3] + A[8]*B[6];
		C[7] = A[6]*B[1] + A[7]*B[4] + A[8]*B[7];
		C[8] = A[6]*B[2] + A[7]*B[5] + A[8]*B[8];

		return C;
	}

	Vector3 Transform(Vector3 const &P) const
	{
		return Vector3
		(
			  P.x*X.x + P.y*X.y + P.z*X.z
			, P.x*Y.x + P.y*Y.y + P.z*Y.z
			, P.x*Z.x + P.y*Z.y + P.z*Z.z
		);
	}

	Vector2 Transform(Vector2 const &P) const
	{
		Vector3 R = Transform(Vector3(P.x, P.y, 1));
//		if (R.z == 0)
			return Vector2(R.x, R.y);
//		return Vector2(R.x/R.z, R.y/R.z);
	}

	static Matrix ScaleRotateTranslate(float sx, float sy, float angle, Point const &translate)
	{
		Matrix scale = Matrix::Scale(sx, sy);
		Matrix rotate = Matrix::Rotation(angle);
		Matrix move = Matrix::Translate(translate);
		return scale.Multiply(rotate).Multiply(move);
	}
};

inline Matrix operator*(Matrix const &A, Matrix const &B)
{
	return A.Multiply(B);
}

inline Vector2 operator*(Matrix const &A, Vector2 const &B)
{
	return A.Transform(B);
}

inline LineSegment Transform(LineSegment const &ls, Matrix const &matrix)
{
	return LineSegment(matrix*ls.first, matrix*ls.second);
}

#endif // MATRIX_H_INCLUDED

//EOF
