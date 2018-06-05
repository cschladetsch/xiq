// (C) 2009 christian.schladetsch@gmail.com

#pragma once

/// @brief The titular enemy of the game, Xiq
///
/// Uses a combination of a wandering behavior,
/// verlet integration, and harmonic radius animation to
/// make things a little interesting for the player
struct Xiq : Object
{
private:
    Point2 last_pos;        ///< used for verlet integration
    float max_speed;
    float mass;
    Vector2 force;

    Vector2 steer;
    float angle;                ///< for wandering
    Sinusoid radius_waves[2];    ///< for radius changes
    Sinusoid colors[3];            ///< for color animation

    bool hit_player;            ///< if true, xiq has hit the player

public:
    void Prepare();

    bool Update(GameTime);
    void Draw(Matrix const &);

    void SetRadius(float);

    bool HasHitPlayer() const { return hit_player; }
    void SetHitPlayer(bool T = true) { hit_player = T; }

protected:
    /// custom SetPixel to provide forces against world elements
    void SetPixel(int x, int y, Color color);
};

