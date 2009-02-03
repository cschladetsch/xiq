// (C) 2009 www.christian.schladetsch.net

#ifndef PHASE_H_INCLUDED
#define PHASE_H_INCLUDED


struct Level;

/// the different phases of the game are represented as different types
namespace Phase
{
	struct State
	{
		enum Type
		{
			None,
			Boot,
			Splash,
			Attract,

			TransitionToPlay,
			Play,
			PostPlay,

			LevelTransition,
		};
		Type type;
	};

	/// common for all phases
	struct Base : Object
	{
		/// optionally override to handle input during the phase
		virtual bool InputEvent(SDL_Event const &) { return false; }

		//virtual void UpdateTransition(GameTime) = 0;
	};

	/// loading/boot phase
	struct Boot : Base
	{
		bool Update(GameTime);
		void Draw();
	};

	/// the attract phase runs the game in a demo loop
	struct Attract : Base
	{
		bool Update(GameTime);
		void Draw();
	};

	/// the main play phase
	struct Play : Base
	{
		Player *player;
		Playfield *playfield;
		Level *level;
		World *world;
		Game *game;

		// overrides for Object
		bool Update(GameTime);
		void Draw(Matrix const &);

		// overrides for Phase
		bool InputEvent(SDL_Event const &);

	protected:
		void PlayerDirects(Direction);
		void PlayerUnDirects(Direction);
	};

	/// animation for moving from one level to another
	struct LevelTransition : Base
	{
		bool Update(GameTime);
		void Draw();
	};

	/// show the end score, high-scores and allow player to enter name
	struct GameOver : Base
	{
		bool Update(GameTime);
		void Draw();
	};
}

#endif // PHASE_H_INCLUDED

//EOF
