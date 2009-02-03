// (C) 2009 www.christian.schladetsch.net

#include "Common.h"
#include "Draw.h"
#include "BresenhamLineDraw.h"

unsigned short *WritePixel(unsigned short *out, Color color, short x, short y)
{
	*out++ = color;
	*out++ = x;
	*out++ = y;
	return out;
}

void DrawLineSegment(SDL_Surface *surface, LineSegment const &line, Color color)
{
	// TODO: clipping
	unsigned short color_x_y[10000];
	unsigned short *pixels = (unsigned short *)surface->pixels;
	unsigned short *end = BresenhamLineDraw(line.first.x, line.first.y, line.second.x, line.second.y, color, color_x_y);
	for (unsigned short *line = color_x_y; line < end; )
	{
		unsigned short color = *line++;
		short x = *line++;
		short y = *line++;
		if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)	// TODO: clipping
			continue;
		pixels[x + y*surface->pitch/2] = color;
	}
}

void SetPixel(SDL_Surface *surface, int x, int y, Color color, BlendMode blend)
{
	if (x < 0 || y < 0 || x >= surface->w  || y >= surface->h)
		return;

	//printf("SetPixel: %d %d\n", x, y);
	char *pixels = (char *)surface->pixels;
	Color *scanline = (Color *)(pixels + surface->pitch*y);
	if (blend == BlendMode::Add)
	{
		Color dest = scanline[x];
		Uint8 dr, dg, db;
		Uint8 sr, sg, sb;
		SDL_GetRGB(dest, surface->format, &dr, &dg, &db);
		SDL_GetRGB(color, surface->format, &sr, &sg, &sb);
		int r = Clamp<int>(dr + sr, 0, 255);
		int g = Clamp<int>(dg + sg, 0, 255);
		int b = Clamp<int>(db + sb, 0, 255);

//		printf("%d %d %d -> %d %d %d\n", dr, dg, db, r,g,b);
		Color q = SDL_MapRGB(surface->format, r, g, b);
		scanline[x] = q;
	}
	else
		scanline[x] = color;
}

void DrawCircle(SDL_Surface *surface, Point::Ordinate x0, Point::Ordinate y0, int radius, Color color, BlendMode blend)
{
	(void)blend;

	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	SetPixel(surface, x0, y0 + radius, color, blend);
	SetPixel(surface, x0, y0 - radius, color, blend);
	SetPixel(surface, x0 + radius, y0, color, blend);
	SetPixel(surface, x0 - radius, y0, color, blend);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		SetPixel(surface, x0 + x, y0 + y, color, blend);
		SetPixel(surface, x0 - x, y0 + y, color, blend);
		SetPixel(surface, x0 + x, y0 - y, color, blend);
		SetPixel(surface, x0 - x, y0 - y, color, blend);
		SetPixel(surface, x0 + y, y0 + x, color, blend);
		SetPixel(surface, x0 - y, y0 + x, color, blend);
		SetPixel(surface, x0 + y, y0 - x, color, blend);
		SetPixel(surface, x0 - y, y0 - x, color, blend);
	}
}

//EOF
