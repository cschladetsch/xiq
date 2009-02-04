// (C) 2009 christian.schladetsch@gmail.com

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

Color GetPixel(SDL_Surface *surface, int x, int y)
{
	if (x < 0 || y < 0 || x >= surface->w  || y >= surface->h)
		return 0;

	char *pixels = (char *)surface->pixels;
	Color *scanline = (Color *)(pixels + surface->pitch*y);
	return scanline[x];
}

void SetPixel(SDL_Surface *surface, int x, int y, Color color, BlendMode blend)
{
	if (x < 0 || y < 0 || x >= surface->w  || y >= surface->h)
		return;

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

void DrawLineSegmentAntiAliased(SDL_Surface *surface, int X0, int Y0, int X1, int Y1, Color color)
{
    // make sure the line runs top to bottom
    if (Y0 > Y1)
    {
        int Temp = Y0; Y0 = Y1; Y1 = Temp;
        Temp = X0; X0 = X1; X1 = Temp;
    }

    /* Draw the initial pixel, which is always exactly intersected by
    the line and so needs no weighting */
    SetPixel(surface, X0, Y0, color);

    int XDir, DeltaX = X1 - X0;
    if( DeltaX >= 0 )
    {
        XDir = 1;
    }
    else
    {
        XDir   = -1;
        DeltaX = 0 - DeltaX; /* make DeltaX positive */
    }

    /* Special-case horizontal, vertical, and diagonal lines, which
    require no weighting because they go right through the center of
    every pixel */
    int DeltaY = Y1 - Y0;
    if (DeltaY == 0)
    {
        /* Horizontal line */
        while (DeltaX-- != 0)
        {
            X0 += XDir;
            SetPixel(surface, X0, Y0, color);
        }
        return;
    }
    if (DeltaX == 0)
    {
        /* Vertical line */
        do
        {
            Y0++;
            SetPixel(surface, X0, Y0, color);
        } while (--DeltaY != 0);
        return;
    }

    if (DeltaX == DeltaY)
    {
        /* Diagonal line */
        do
        {
            X0 += XDir;
            Y0++;
            SetPixel(surface, X0, Y0, color);
        } while (--DeltaY != 0);
        return;
    }

    unsigned short ErrorAdj;
    unsigned short ErrorAccTemp, Weighting;

    /* Line is not horizontal, diagonal, or vertical */
    unsigned short ErrorAcc = 0;  /* initialize the line error accumulator to 0 */

    Uint8 rl, gl, bl;
	SDL_GetRGB(color, surface->format, &rl, &gl, &bl);

    double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;

    /* Is this an X-major or Y-major line? */
    if (DeltaY > DeltaX)
    {
    /* Y-major line; calculate 16-bit fixed-point fractional part of a
    pixel that X advances each time Y advances 1 pixel, truncating the
        result so that we won't overrun the endpoint along the X axis */
        ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
        /* Draw all pixels other than the first and last */
        while (--DeltaY)
        {
            ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
            ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
            if (ErrorAcc <= ErrorAccTemp)
            {
                /* The error accumulator turned over, so advance the X coord */
                X0 += XDir;
            }
            Y0++; /* Y-major, so always advance Y */
                  /* The IntensityBits most significant bits of ErrorAcc give us the
                  intensity weighting for this pixel, and the complement of the
            weighting for the paired pixel */
            Weighting = ErrorAcc >> 8;
            assert( Weighting < 256 );
            assert( ( Weighting ^ 255 ) < 256 );

            Color clrBackGround = GetPixel(surface, X0, Y0 );
            Uint8 rb, gb, bb;
            SDL_GetRGB(clrBackGround, surface->format, &rb, &gb, &bb);

            double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

            Uint8 rr = ( rb > rl ? ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:
		      (Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) :
		      ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
		      / 255.0 * ( rl - rb ) + rb ) ) );
            Uint8 gr = ( gb > gl ? ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:
		      (Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) :
		      ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
		      / 255.0 * ( gl - gb ) + gb ) ) );
            Uint8 br = ( bb > bl ? ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:
		      (Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) :
		      ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
		      / 255.0 * ( bl - bb ) + bb ) ) );

            SetPixel(surface, X0, Y0, SDL_MapRGB(surface->format, rr, gr, br ) );

            clrBackGround = GetPixel(surface, X0 + XDir, Y0 );
            SDL_GetRGB(clrBackGround, surface->format, &rb, &gb, &bb);
            grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

            rr = ( rb > rl ? ( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):
		Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
		/ 255.0 * ( rl - rb ) + rb ) ) );
            gr = ( gb > gl ? ( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):
		Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
		/ 255.0 * ( gl - gb ) + gb ) ) );
            br = ( bb > bl ? ( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):
		Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
		/ 255.0 * ( bl - bb ) + bb ) ) );
            SetPixel(surface, X0 + XDir, Y0, SDL_MapRGB(surface->format, rr, gr, br ) );
        }
        /* Draw the final pixel, which is always exactly intersected by the line
        and so needs no weighting */
        SetPixel(surface, X1, Y1, color );
        return;
    }
    /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
    pixel that Y advances each time X advances 1 pixel, truncating the
    result to avoid overrunning the endpoint along the X axis */
    ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
    /* Draw all pixels other than the first and last */
    while (--DeltaX)
    {
        ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
        ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
        if (ErrorAcc <= ErrorAccTemp)
        {
            /* The error accumulator turned over, so advance the Y coord */
            Y0++;
        }
        X0 += XDir; /* X-major, so always advance X */
                    /* The IntensityBits most significant bits of ErrorAcc give us the
                    intensity weighting for this pixel, and the complement of the
        weighting for the paired pixel */
        Weighting = ErrorAcc >> 8;
        assert( Weighting < 256 );
        assert( ( Weighting ^ 255 ) < 256 );

		Color clrBackGround = GetPixel(surface, X0, Y0 );
		Uint8 rb, gb, bb;
		SDL_GetRGB(clrBackGround, surface->format, &rb, &gb, &bb);
        double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

        Uint8 rr = ( rb > rl ? ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:
		(Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
		/ 255.0 * ( rl - rb ) + rb ) ) );
        Uint8 gr = ( gb > gl ? ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:
		(Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
		/ 255.0 * ( gl - gb ) + gb ) ) );
        Uint8 br = ( bb > bl ? ( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:
		(Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) )
		/ 255.0 * ( bl - bb ) + bb ) ) );

        SetPixel(surface, X0, Y0, SDL_MapRGB(surface->format, rr, gr, br ) );

		clrBackGround = GetPixel(surface, X0, Y0 );
		SDL_GetRGB(clrBackGround, surface->format, &rb, &gb, &bb);
        grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

        rr = ( rb > rl ? ( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):
		Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
		/ 255.0 * ( rl - rb ) + rb ) ) );
        gr = ( gb > gl ? ( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):
		Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
		/ 255.0 * ( gl - gb ) + gb ) ) );
        br = ( bb > bl ? ( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):
		Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) :
		( ( Uint8 )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) )
		/ 255.0 * ( bl - bb ) + bb ) ) );

        SetPixel(surface, X0, Y0 + 1, SDL_MapRGB(surface->format, rr, gr, br ) );
    }

    /* Draw the final pixel, which is always exactly intersected by the line
    and so needs no weighting */
    SetPixel(surface, X1, Y1, color );
}

//EOF
