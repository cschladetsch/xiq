// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
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

int Playfield::CalcNewArea(Direction dir, Point2 P)
{
    Point2 A = P, B = P

    switch (dir.value)
    {
        case Direction::Left:  A += Vector2(1,-1); B += Vector2(1,1); break;
        case Direction::Right: A += Vector2(-1,-1); B += Vector2(-1,1); break;
        case Direction::Up:    A += Vector2(-1,1); B += Vector2(1,1); break;
        case Direction::Down:  A += Vector2(-1,-1); B += Vector2(1,-1); break;
        default: break;
    }

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

    printf("CalcNewArea: %f %f = %d (%f filled)\n", P.x, P.y, num_filled, percent_filled);
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

bool Playfield::OutOfBounds(Point2 p) const
{
    return p.x < 0 || p.y < 0 || p.x >= width || p.y >= height;
}

bool Playfield::FloodFillCompare(Point2 A, Point2 B)
{
    // LOL: this is *extremely* inefficient; the entire playfield is copied three times!!
    Elements tmp = elements;
    int na = FloodFill(A, Filled);
    elements = tmp;
    int nb = FloodFill(B, Filled);
    elements = tmp;
    return na < nb;
}

struct Segment { short y, xl, xr, dy; };

int Playfield::FloodFill(int x, int y, Element nv)
{
    #define MAX 10000        // max depth of stack

    #define PUSH(Y, XL, XR, DY)    \
        if (sp < stack + MAX && Y + (DY) >= 0 && Y+(DY)< height) \
        {sp->y = Y; sp->xl = XL; sp->xr = XR; sp->dy = DY; sp++;}

    #define POP(Y, XL, XR, DY) \
        {sp--; Y = sp->y+(DY = sp->dy); XL = sp->xl; XR = sp->xr;}

    int l, x1, x2, dy;
    Element ov;                            // old value
    Segment stack[MAX], *sp = stack;    // stack of filled segments

    int num_filled = 0;
    if (x < 0 || x >= width || y < 0 || y > height - 1)
        return num_filled;

    ov = At(x, y);                        // read ov at seed point
    if (ov != Empty)
        return num_filled;

    PUSH(y, x, x, 1);
    PUSH(y+1, x, x, -1);                // seed segment (popped 1st)

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
            PUSH(y, l, x1-1, -dy);        /* leak on left? */
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
                PUSH(y, x2 + 1, x - 1, -dy);    // leak on right? */
skip:        for (x++; x < width && At(x, y) != ov; x++)
            {
                // do nothing
            }
            l = x;
        }
        while (x <= x2);
    }
    return num_filled;
}

Point2 *Playfield::LineDraw(Point2 p0, Point2 p1, Point2 *out) const
{
    float vx = p1.x - p0.x;
    float vy = p1.y - p0.y;

    int nx = abs(vx);            // length to move in x
    int ny = abs(vy);            // length to move in y
    int dx = vx > 0 ? 1 : -1;    // direction of x
    int dy = vy > 0 ? 1 : -1;    // direction of y
    if (ny == 0)                 // horizontal
    {
        for (; nx > 0; --nx)
        {
            *out++ = p0;
            p0.x += dx;
            if (p0.x < 0 || p0.x >= width)
                break;
        }
    }
    else                        // vertical
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

    Color green = SDL_MapRGB(surface->format, 0,255,0);
    Color blue = SDL_MapRGB(surface->format, 0,0,255);
    Color white = SDL_MapRGB(surface->format, 255,255,255);
    Color gray = SDL_MapRGB(surface->format, 30,30,50);
    Color cyan = SDL_MapRGB(surface->format, 50,50,100);

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
