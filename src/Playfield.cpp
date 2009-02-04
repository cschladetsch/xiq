// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "BresenhamLineDraw.h"
#include "Draw.h"
#include "Game.h"

void Playfield::Create(int _width, int _height)
{
	width = _width;
	height = _height;
	elements.resize(width*height, Empty);
	Clear();
}

void Playfield::Clear()
{
	percent_filled = 0;

	std::fill(elements.begin(), elements.end(), Empty);

	// prepare the boundaries
	for (int x = 0; x < width; ++x)
	{
		Set(x,0, Line);
		Set(x, height - 1, Line);
	}
	for (int y = 0; y < height; ++y)
	{
		Set(0, y, Line);
		Set(width - 1, y, Line);
	}
}

bool Playfield::Update(GameTime)
{
	return true;
}

void Playfield::MovePlayer(Player &player, GameTime time, Element what)
{
	if (!player.Moving())
		return;

	// determine the path of movement
	Direction dir = player.GetDirection();
	Vector v = dir.GetVector();
	float speed = player.GetSpeed();
	Point location = player.GetLocation();
	Point end_pos = location + v*speed*time.DeltaMillis();

	// something is wrong with movement... it is jerky and slows down on horizontals??
//	if (dir == Direction::Left) end_pos += Vector(-1,0);
//	if (dir == Direction::Right) end_pos += Vector(1,0);
	LineSegment movement(location, end_pos);

	// rasterise the line-segement
	Point buffer[10*1000];
	Point *start = buffer;
	Point *end = LineDraw(movement.first, movement.second, buffer);

	bool drawing = player.IsDrawing();

	// traverse the points
	while (start != end)
	{
		Point next = *start++;
		Element e_next = At(next);

		if (OutOfBounds(next))
		{
			break;
		}

		// can't move into filled locations
		if (e_next == Filled)
		{
			break;
		}

		// skip the position that the player is currently on
		if (player.GetLocation() == next)
		{
			goto next_location;
		}

		// can't go through an existing new line
		if (drawing && e_next == NewLine)
		{
			break;
		}

		// can only move on lines when not creating new one
		if (!drawing && e_next != Line)
		{
			break;
		}

		// complete an area if we are drawing and hit an existing line
		if (drawing && e_next == Line)
		{
			int num_filled = CalcNewArea(dir, next);
			double score = (double )num_filled*num_filled/10000000.;
			player.AddScore((int)score);
			player.SetDrawing(false);
			return;
		}

		// test for movement into a perpendicular line
		if (!drawing)
		{
			for (int n = Direction::Left; n < Direction::Last; ++n)
			{
				Direction dir = Direction::Type(n);
				if (!player.WantsDirection(dir))
					continue;
				Point P = next + dir.GetVector();
				if (OutOfBounds(P))
					continue;
				if (At(P) == Line)
				{
					// change movement
					player.SetDirection(dir);
					player.SetLocation(next);
					return;
				}
			}
		}

next_location:
		if (e_next == Filled)
			break;
		Set(next, what);
		player.SetLocation(next);
	}
}

// calculate a new area to fill after the player completes a loop on the playfield
// dir is the direction the player was travelling, and P is the point that completed the loop
int Playfield::CalcNewArea(Direction dir, Point P)
{
	Point A = P, B = P;		// seeds for two prospective new areas

	// depending on the direction we travelled to get here,
	// calculate the two seed points
	switch (dir.value)
	{
		case Direction::Left: 	A += Vector(1,-1); B += Vector(1,1); break;
		case Direction::Right: 	A += Vector(-1,-1); B += Vector(-1,1); break;
		case Direction::Up: 	A += Vector(-1,1); B += Vector(1,1); break;
		case Direction::Down: 	A += Vector(-1,-1); B += Vector(1,-1); break;
		default: break;
	}

	// see which side produced the smallest resulting new area
	bool result = FloodFillCompare(A, B);
	int num_filled = 0;
	if (result)
	{
		num_filled = FloodFill(A, Filled);
	}
	else
	{
		num_filled = FloodFill(B, Filled);
	}

	RemoveNewLines();

	float total = width*height;
	float percent = (float)num_filled/total;
	percent_filled += percent;

	return num_filled;
}

void Playfield::RemoveNewLines(Element what)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (At(x, y) == NewLine)
				Set(x, y, what);
		}
	}
}

bool Playfield::OutOfBounds(Point p) const
{
	return p.x < 0 || p.y < 0 || p.x >= width || p.y >= height;
}

bool Playfield::FloodFillCompare(Point A, Point B)
{
	// LOL: this is *extremely* inefficient; the entire playfield is copied three times!!
	Elements tmp = elements;
	int na = FloodFill(A, Filled);
	elements = tmp;
	int nb = FloodFill(B, Filled);
	elements = tmp;
//	printf("FloodFillCompare: %d %d from (%f %f) and (%f %f)\n", na, nb, A.x, A.y, B.x, B.y);
	return na < nb;
}

// Filled horizontal segment of scanline y for xl<=x<=xr.
// Parent segment was on line y-dy.  dy=1 or -1
struct Segment { short y, xl, xr, dy; };

// perform a very fast, non-recursive flood-fill given a seed point
int Playfield::FloodFill(int x, int y, Element nv)
{
	#define MAX 10000		// max depth of stack

	// push new segment on stack
	#define PUSH(Y, XL, XR, DY)	\
		if (sp < stack + MAX && Y + (DY) >= 0 && Y+(DY)< height) \
		{sp->y = Y; sp->xl = XL; sp->xr = XR; sp->dy = DY; sp++;}

	// pop segment off stack
	#define POP(Y, XL, XR, DY) \
		{sp--; Y = sp->y+(DY = sp->dy); XL = sp->xl; XR = sp->xr;}

    int l, x1, x2, dy;
    Element ov;							// old value
    Segment stack[MAX], *sp = stack;	// stack of filled segments

	int num_filled = 0;
	if (x < 0 || x >= width || y < 0 || y > height - 1)
		return num_filled;

    ov = At(x, y);						// read ov at seed point
    if (ov != Empty)
		return num_filled;

    PUSH(y, x, x, 1);
    PUSH(y+1, x, x, -1);				// seed segment (popped 1st)

    while (sp > stack)
    {
		// pop segment off stack and fill a neighboring scan line */
		POP(y, x1, x2, dy);
		// segment of scan line y-dy for x1<=x<=x2 was previously filled,
		// now explore adjacent pixels in scan line y
		for (x = x1; x >= 0 && At(x, y) == ov; x--)
		{
			num_filled++;
			Set(x, y, nv);
		}

		if (x >= x1)
			goto skip;
		l = x + 1;
		if (l < x1)
			PUSH(y, l, x1-1, -dy);		/* leak on left? */
		x = x1 + 1;
		do
		{
			for (; x < width && At(x, y) == ov; x++)
			{
				Set(x, y, nv);
				num_filled++;
			}

			PUSH(y, l, x-1, dy);
			if (x > x2 + 1)
				PUSH(y, x2 + 1, x - 1, -dy);	// leak on right? */
skip:		for (x++; x < width && At(x, y) != ov; x++)
			{
				// do nothing
			}
			l = x;
		}
		while (x <= x2);
    }
    return num_filled;
}

Point *Playfield::LineDraw(Point p0, Point p1, Point *out) const
{
	int vx = p1.x - p0.x;
	int vy = p1.y - p0.y;
	int nx = abs(vx);			// length to move in x
	int ny = abs(vy);			// length to move in y
	int dx = vx > 0 ? 1 : -1;	// direction of x
	int dy = vy > 0 ? 1 : -1;	// direction of y
	if (ny == 0)				// horizontal
	{
		for (; nx > 0; --nx)
		{
			*out++ = p0;
			p0.x += dx;
			if (p0.x < 0 || p0.x >= width)
				break;
		}
	}
	else						// vertical
	{
		for (; ny > 0; --ny)
		{
			*out++ = p0;
			p0.y += dy;
			if (p0.y < 0 || p0.y >= height)
				break;
		}
	}
	return out;
}

// traverse over the playfield, writing colors to the surface
// that distinguish the different playfield element states (filled, line etc)
void Playfield::Draw(Matrix const &)
{
	SDL_Surface *surface = GetRoot()->GetSurface();

//	Color red = SDL_MapRGB(surface->format, 255,0,0);
	Color green = SDL_MapRGB(surface->format, 0,255,0);
	Color blue = SDL_MapRGB(surface->format, 0,0,255);
	Color white = SDL_MapRGB(surface->format, 255,255,255);
	Color gray = SDL_MapRGB(surface->format, 100,100,100);
	Color cyan = SDL_MapRGB(surface->format, 200,0,200);

	for (int y = 0; y < height; ++y)
	{
		Element *element = &elements.at(y*width);
		Unsigned16 *pixel = (Unsigned16 *)((char *)surface->pixels + y*surface->pitch);
		for (int x = 0; x < width; ++x, ++element, ++pixel)
		{
			switch (*element)
			{
				case Edge:
					*pixel = white;
					break;
				case Filled:
					*pixel =cyan;
					break;
				case Line:
					*pixel = green;
					break;
				case NewLine:
					*pixel = blue;
					break;
				case Empty:
					*pixel = gray;
					break;
				default:
					break;
			}
		}
	}
}

//EOF
