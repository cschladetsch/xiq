// (C) 2009 www.christian.schladetsch.net

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Phase.h"

/// @brief the toplevel game object.
///
/// stores the object factory for all other objects in the system.
/// manages the phase transitions and controls the main processing loop
struct Game
{
	Factory *factory;		///< source of all game objects: static and dynamic
	int level;				///< current level
	World *world;			///< current world. stores all dynamic game objects and the playfield
	Player *player;			///< the player
	bool initialised;
	bool finished;
	bool space_down;
	Phase::Base *phase;		///< the current phase of the game
	Font *font;

public:
	Game(int width, int height);
	~Game();

	// start a new game
	void Start();
	void StartLevel();
	void NextLevel();
	void End();

	template <class T>
	T *New()
	{
		T *obj = factory->template New<T>();
		obj->game = this;
		return obj;
	}

	void Update();
	void Draw();

	Time TimeNow() const;

	bool Finished() const { return finished; }
	bool Initialised() const { return initialised; }

	World *GetWorld() const;
	SDL_Surface *GetSurface() const;
	Player *GetPlayer() const { return player; }
	GameTime GetTime() const;
	Color MakeColor(int r, int g, int b) const;

	void RestartLevel();

protected:
	void UpdateHUD();
	void ParseInput();

	/// deal with intention of player to move in the given direction
	void PlayerDirects(Direction);

	/// player stops wanting the given direction
	void PlayerUnDirects(Direction);
};

#endif // GAME_H_INCLUDED

//EOF
