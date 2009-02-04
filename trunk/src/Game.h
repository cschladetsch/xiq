// (C) 2009 christian.schladetsch@gmail.com

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
	Level *level;			///< the current level; used to prepare
	World *world;			///< current world. stores all dynamic game objects and the playfield
	Player *player;			///< the player
	GameTime time;

	SDL_Surface *screen;

	bool initialised;
	bool finished;

	Phase::Base *phase;			///< the current phase of the game
	Phase::Base *next_phase;	///< the phase to transition to
	Time transition_ends;		///< when the current transition ends

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
	Font *GetFont() const { return font; }
	GameTime GetTime() const;

	Color MakeColor(int r, int g, int b) const;

	/// move to a new phase
	void PhaseChange(Phase::Base *next_phase, Time transition_time = 0);

protected:
	void UpdateHUD();
	void ParseInput();

	/// deal with intention of player to move in the given direction
	void PlayerDirects(Direction);

	/// player stops wanting the given direction
	void PlayerUnDirects(Direction);

	/// returns true iff the game is currently moving between phases
	bool Transitioning() const;

	/// perform transition between levels
	void Transist();

	/// end the current transition now
	void EndTransition();

private:
	/// register game types with the object factory
	void RegisterTypes();

	/// prepare the SDL system
	bool InitialiseSDL(int width, int height);
};

#endif // GAME_H_INCLUDED

//EOF
