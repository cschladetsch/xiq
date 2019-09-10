// (C) 2009 christian.schladetsch@gmail.com

/// @file Draw routines for anti-aliased line, circle and fill
#pragma once

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
void DrawLineSegmentAntiAliased(SDL_Surface *surface, int x0, int y0, int x1, int y1, Color color);

void DrawLineSegment(SDL_Surface *surface, LineSegment const &line, Color color);
void DrawCircle(SDL_Surface *surface, Point2::Ordinate x0, Point2::Ordinate y0, int radius, Color color, BlendMode = BlendMode());

inline void DrawLineSegmentAntiAliased(SDL_Surface *surface, LineSegment const &line, Color color)
{
    DrawLineSegmentAntiAliased(surface, line.first.x, line.first.y, line.second.x, line.second.y, color);
}

