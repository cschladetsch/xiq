// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"

#include "Draw.h"

void Player::Prepare()
{
    collides = true;
    speed = 180;
    radius = 20;
    drawing = false;
    std::fill(wants_direction, wants_direction + 5, false);
    num_lives = 3;
    Time now = GetRoot()->TimeNow();
    immunity_ends = now + 2;
    respawn_ends = now + 2;
    score = 0;
    location = Point2(300,399);
}

void Player::LoseLife()
{
    if (num_lives == 0)
        return;

    --num_lives;
    bool dead = num_lives == 0;

    Time now = GetRoot()->TimeNow();
    respawn_ends = now + (dead ? 2 : 1);
    immunity_ends = now + 2;

    SetDirection(Direction::None);
    if (IsDrawing() && !dead)
    {
        drawing = false;
        location = launch_point;
    }

    if (!dead)
    {
        GetPlayfield()->RemoveNewLines(Playfield::Empty);
    }
}

void Player::Move(LineSegment const &traversed)
{
    location = traversed.second;
}

void Player::SetDirection(Direction dir)
{
    if (dir == direction)
        return;
    direction = dir;
}

void Player::SetWantsDirection(Direction dir, bool wants)
{
    if (dir == direction)
        SetDirection(Direction::None);

    wants_direction[dir.value] = wants;
    if (wants == false)
    {
        for (int n = 0; n < 4; ++n)
        {
            if (wants_direction[n])
            {
                SetDirection(Direction::Type(n));
                return;
            }
        }
    }
}

void Player::SetLocation(Point2 P)
{
    location = P;
}

bool Player::Update(GameTime time)
{
    if (!Moving() || num_lives == 0)
        return true;

    Playfield *playfield = GetPlayfield();

    Direction dir = GetDirection();
    Vector2 v = dir.GetVector();
    float speed = GetSpeed();
    double distance = speed*time.DeltaSeconds();

    Playfield::Element what = drawing ? Playfield::NewLine : Playfield::Line;

    distance = 1;

    while (distance > 0)
    {
        double step_dist = Clamp(distance, 0.0, 1.0);
        Point2 next = location + v*step_dist;

        Playfield::Element e_next = playfield->At(next);

        if (playfield->OutOfBounds(next))
            break;

        if (e_next == Playfield::Filled)
            break;

        if (drawing && e_next == Playfield::NewLine)
            break;

        if (!drawing && e_next != Playfield::Line)
            break;

        if (drawing && e_next == Playfield::Line)
        {
            int num_filled = playfield->CalcNewArea(dir, next);
            double score = (double )num_filled*num_filled/10000000.;
            SetLocation(next);
            AddScore((int)score);
            SetDrawing(false);
            return true;
        }

        if (!drawing)
        {
            for (int n = Direction::Left; n < Direction::Last; ++n)
            {
                Direction dir = Direction::Type(n);
                if (!WantsDirection(dir))
                    continue;
                Point2 P = next + dir.GetVector();
                if (playfield->OutOfBounds(P))
                    continue;
                if (playfield->At(P) == Playfield::Line)
                {
                    // change movement.
                    // we should really continue along the new
                    // path, because as it stands the player will
                    // move a shorter distance when cornering.
                    // but i doubt it will matter.
                    SetDirection(dir);
                    SetLocation(location + v);
                    return true;
                }
            }
        }

        playfield->Set(next, what);

        distance -= step_dist;

        SetLocation(next);
    }
    return true;
}

void Player::AddScore(int s)
{
    score += s;
}

bool Player::IsImmune() const
{
    return GetRoot()->TimeNow() < immunity_ends;
}

bool Player::IsRespawning() const
{
    return GetRoot()->TimeNow() < respawn_ends;
}

bool Player::Moving() const
{
    return direction != Direction::None && !IsRespawning();
}

void Player::Draw(Matrix const &)
{
    if (IsRespawning())
    {
        DrawRespawn();
    }
    if (HasNoLives())
    {
        return;
    }

    Point2 P = GetLocation();
    float L = GetRadius()/2.0f;

    Matrix matrix = Matrix::ScaleRotateTranslate(L,L,0,P);
    Matrix shadow = Matrix::ScaleRotateTranslate(L,L,0,P + Vector2(2,2));

    Color color = IsImmune() ? GetRoot()->MakeColor(255,255,0) : GetRoot()->MakeColor(0,255,255);

    Draw(shadow, 0);
    Draw(matrix, color);
}

void Player::Draw(Matrix const &M, Color color)
{
    Point2 points[] =
    {
        Point2(-1, 0),
        Point2(0, -1),
        Point2(1, 0),
        Point2(0, 1),
    };
    for (int n = 0; n < 4; ++n)
    {
        points[n] = M*points[n];
    }
    SDL_Surface *surface = GetRoot()->GetSurface();
    for (int n = 0; n < 4; ++n)
    {
        DrawLineSegment(surface, LineSegment(points[n], points[(n + 1)%4]), color);
    }
}

void Player::DrawRespawn()
{
    float remaining = respawn_ends - GetRoot()->TimeNow();
    float alpha = 1 + 300*remaining*remaining;

    float radius = alpha;
    bool dead = HasNoLives();

    if (dead)
    {
        radius = (remaining/2.0f)*500;//radius = 300 - alpha;
    }

    float C = 255;

    Color color1 = GetRoot()->MakeColor(C,0,0);
    Color color2 = GetRoot()->MakeColor(0,C,0);
    Color color3 = GetRoot()->MakeColor(0,0,C);

    if (dead)
    {
        color2 = GetRoot()->MakeColor(C,C,0);
        color3 = GetRoot()->MakeColor(C,C,C);
    }
    SDL_Surface *surface = GetRoot()->GetSurface();
    for (int n = 0; n < 20; ++n, radius *= 0.80f)
    {
        DrawCircle(surface, location.x - 1, location.y - 1, radius + 1, color1);
        DrawCircle(surface, location.x + 0, location.y + 0, radius + 0, color2);
        DrawCircle(surface, location.x + 1, location.y - 1, radius - 1, color3);
    }
}
