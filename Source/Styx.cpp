// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "Object.h"
#include "Styx.h"
#include "Draw.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"
#include "BresenhamLineDraw.h"

Styx::Styx()
{
    collides = true;
    move_toward_player = true;
}

bool Styx::Update(GameTime time)
{
    Playfield const &pf = *GetPlayfield();

    std::vector<Direction> choices;
    for (int n = 0; n < 4; ++n)
    {
        Direction dir = (Direction::Type)n;

        if (dir.Opposite() == direction)
            continue;

        Playfield::Element element = pf.At(location + dir.GetVector());
        if (element == Playfield::Line)
        {
            choices.push_back(Direction::Type(n));
        }
    }

    if (choices.empty())
    {
        direction = direction.Opposite();
    }
    else
    {
        SetDirection(choices[rand() % choices.size()]);

        if (move_toward_player && choices.size() > 1)
        {
            Point2 pos_player = GetRoot()->GetWorld()->GetPlayer()->GetLocation();
            float min_dist = 0;
            bool first = true;
            for (auto dir : choices)
            {
                float dist = (location + dir.GetVector() - pos_player).Length();
                if (first || dist < min_dist)
                {
                    first = false;
                    min_dist = dist;
                    SetDirection(dir);
                }
            }
        }
    }

    location += direction.GetVector();// TODO *speed* time.DeltaSeconds();
    return true;
}

void Styx::Draw(Matrix const &)
{
    float freq = 2.2f;
    float angle = GetRoot()->TimeNow()*freq;
    float radius = GetRadius();

    Point2 location = GetLocation();
    Point2 shadow = location - Vector2(2,2);
    Draw(Matrix::ScaleRotateTranslate(radius, radius, angle, shadow), 0);
    Draw(Matrix::ScaleRotateTranslate(radius*0.75, radius*0.75, angle*1.5, shadow), 0);
    Draw(Matrix::ScaleRotateTranslate(radius*0.4, radius*0.4, angle*2.5, shadow), 0);

    Draw(Matrix::ScaleRotateTranslate(radius, radius, angle, location), color);
    Draw(Matrix::ScaleRotateTranslate(radius*0.75, radius*0.75, angle*1.5, location), color);
    Draw(Matrix::ScaleRotateTranslate(radius*0.4, radius*0.4, angle*2.5, location), color);
}

void Styx::Draw(Matrix const &M, Color color)
{
    float F = 1;
    Point2 points[4] =
    {
        Point2(-F,-F), Point2(F,F),
        Point2(-F,+F), Point2(F,-F)
    };
    for (int n = 0; n < 4; ++n)
    {
        points[n] = M.Transform(points[n]);
    }

    LineSegment L1(points[0], points[1]);
    LineSegment L2(points[2], points[3]);

    DrawLineSegment(GetRoot()->GetSurface(), L1, color);
    DrawLineSegment(GetRoot()->GetSurface(), L2, color);
}

void Styx::DrawLine(LineSegment const &line, Color color)
{
    SDL_Surface *surface = GetRoot()->GetSurface();

    unsigned short color_x_y[10000];
    unsigned short *pixels = (unsigned short *)surface->pixels;
    unsigned short *end = BresenhamLineDraw(line.first.x, line.first.y, line.second.x, line.second.y, color, color_x_y);
    for (unsigned short *line = color_x_y; line < end; )
    {
        unsigned short color = *line++;
        short x = *line++;
        short y = *line++;
        if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)    // TODO: clipping
            continue;
        pixels[x + y*surface->pitch/2] = color;
    }
}
