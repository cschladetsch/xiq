// (C) 2009 christian.schladetsch@gmail.com

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

/// The player and its state
struct Player : Object
{
	bool drawing;
	bool wants_direction[5];
	int num_lives;

	float immunity_ends;	// when immunity ends
	float respawn_ends;		// when respawning ends
	Point launch_point;		// where we started making current area

	int score;

public:
	void Prepare();

	int GetNumLives() const { return num_lives; }
	void SetNumLives(int N) { num_lives = N; }
	void LoseLife();
	bool HasNoLives() const { return num_lives == 0; }

	/// set the time when the player's immunity/respawn ends
	void SetImmune(float T) { immunity_ends = T; }
	void SetRespawn(float T) { respawn_ends = T; }

	/// return true if player is currently immune/respawning
	bool IsImmune() const;
	bool IsRespawning() const;

	/// return true if the player is currently making a new area
	bool IsDrawing() const { return drawing; }

	/// change the current draw state of the player
	void SetDrawing(bool D = true) { drawing = D; launch_point = location; }

	/// move the player to the endpoint of a new line segment
	void Move(LineSegment const &);

	/// change the direction the player is moving
	void SetDirection(Direction);
	void SetLocation(Point P);

	/// return true if the player wants to move in the given direction
	bool WantsDirection(Direction D) const { return wants_direction[D.value]; }
	void SetWantsDirection(Direction D, bool F = true);// { wants_direction[D.value] = F; }

	/// overloads for Object
	bool Update(GameTime);
	void Draw(Matrix const &);
	bool Moving() const;

	int GetScore() { return score; }
	void AddScore(int);
	void SetScore(int N) { score = N; }

protected:
	void DrawRespawn();

	void Draw(Matrix const &M, Color color);

};

#endif // PLAYER_H_INCLUDED

//EOF
