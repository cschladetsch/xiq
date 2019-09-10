// (C) 2009 christian.schladetsch@gmail.com

#pragma once

/// These enemies wander around on created line segments
struct Styx : Object
{
    Sinusoid rot;
    bool move_toward_player;

    Styx();
    bool Update(GameTime);
    void Draw(Matrix const &);

protected:
    void Draw(Matrix const &M, Color);
    void DrawLine(LineSegment const &, Color);
};
