// (C) 2009 christian.schladetsch@gmail.com

#pragma once

/// @brief a description of a level in the game
///
/// contains the initial conditions for the layout of objects on
/// a playfield
struct Level : Object
{
    Time time_last_impact;
    Time min_impact_time;

    Level();

    // Object overloads
    bool Update(GameTime);
    void Draw(Matrix const &);

    // list<object construct params>
    void Start();
};

