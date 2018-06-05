// (C) 2009 christian.schladetsch@gmail.com

#pragma once

/// A direction: none, up, down, left or right
struct Direction
{
    enum Type { Left, Right, Up, Down, None, Last };
    Type value;

    Direction(Type T = None) : value(T) { }

    bool operator==(Direction dir) const { return value == dir.value; }
    bool operator!=(Direction dir) const { return value != dir.value; }

    /// return a unit vector in the current direction
    Vector2 GetVector() const
    {
        switch (value)
        {
            case Left: return Vector2(-1,0);
            case Right: return Vector2(1,0);
            case Up: return Vector2(0,-1);
            case Down: return Vector2(0,1);
            default: break;
        }
        return Vector2(0,0);
    }

    /// return the opposite to the current direction
    Direction Opposite() const
    {
        switch (value)
        {
            case Left: return Right;
            case Right: return Left;
            case Up: return Down;
            case Down: return Up;
            default: break;
        }
        return None;
    }
};

