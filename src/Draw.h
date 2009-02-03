// (C) 2009 www.christian.schladetsch.net

/// @file Draw routines for anti-aliased line, circle and fill

#ifndef DRAW_H_INCLUDED
#	define DRAW_H_INCLUDED

/// Specifies the arithemtic operation used when writing pixels in a draw call
struct BlendMode
{
	enum Type
	{
		Replace,
		Add,
		Multiply
	};
	Type type;
	BlendMode(Type T = Replace) : type(T) { }

	bool operator==(Type T) const { return T == type; }
};

void SetPixel(SDL_Surface *surface, int x, int y, Color color, BlendMode = BlendMode());

Unsigned16 *WritePixel(Unsigned16 *out, Color color, short x, short y);

void DrawLineSegment(SDL_Surface *surface, LineSegment const &line, Color color);

void DrawCircle(SDL_Surface *surface, Point::Ordinate x0, Point::Ordinate y0, int radius, Color color, BlendMode = BlendMode());


#endif // DRAW_H_INCLUDED

//EOF
