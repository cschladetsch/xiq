// (C) 2009 www.christian.schladetsch.net

#ifndef XIQ_H_INCLUDED
#define XIQ_H_INCLUDED

/// @brief The titular enemy of the game, Xiq
///
/// Uses a combination of a wandering behavior,
/// verlet integration, and harmonic radius animation to
/// make things a little interesting for the player
struct Xiq : Object
{
private:
	Point last_pos;		///< used for verlet integration
	float max_speed;
	float mass;
	Vector force;

	Vector steer;
	float angle;				///< for wandering
	Sinusoid radius_waves[2];	///< for radius changes
	Sinusoid colors[3];			///< for color animation

	bool hit_player;			///< if true, xiq has hit the player

public:
	Xiq();

	bool Update(GameTime);
	void Draw(Matrix const &);

	void SetRadius(float);

	bool HasHitPlayer() const { return hit_player; }
	void SetHitPlayer(bool T = true) { hit_player = T; }

protected:
	/// custom SetPixel to provide forces against world elements
	void SetPixel(int x, int y, Color color);
};

#endif // XIQ_H_INCLUDED

//EOF
