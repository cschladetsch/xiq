// (C) 2009 christian.schladetsch@gmail.com

#ifndef WORLD_H_INCLUDED
#	define WORLD_H_INCLUDED

/// The state of the game world
///
/// the playfield, the player and other objects such as enemies and effects
struct World : Object
{
	typedef std::set<Object *> Objects;		///< set of dynamic objects that require update and draw each frame

private:
	Objects objects;
	Playfield *playfield;
	Player *player;

	float time_last_impact;		///< the time that the last impact was added
	float min_impact_time;		///< minimum time between impacts

public:
	void Construct(int width, int height);
	~World();

	// overloads from Object
	bool Update(GameTime);
	void Draw(Matrix const &);

	void AddObject(Object *Q) { objects.insert(Q); }

	Playfield *GetPlayfield() { return playfield; }
	Player *GetPlayer() { return player; }
	Box GetBounds() const;
	Point GetMidPoint() const;

	/// reset the player and playfield
	void Clear();

	/// add a new impact object at the given location and size
	void AddImpact(int x, int y, float radius);

protected:
	void UpdateObjects(GameTime);
	void CollisionDetection();
	void DeleteObjects();

	void DrawObjects(Matrix const &M);
};

#endif // WORLD_H_INCLUDED

//EOF
