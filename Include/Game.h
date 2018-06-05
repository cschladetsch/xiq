// (C) 2009 christian.schladetsch@gmail.com

#pragma once

#include "Phase.h"

/// @brief the toplevel game object.
///
/// stores the object factory for all other objects in the system.
/// manages the phase transitions and controls the main processing loop
struct Game : Object
{
    GameTime time;

    SDL_Window* window;
    SDL_Renderer *renderer;
    SDL_Surface *screen;
    bool initialised;
    bool finished;

    Phase::Base *phase;            ///< the current phase of the game
    Phase::Base *next_phase;    ///< the phase to transition to
    Time transition_ends;        ///< when the current transition ends

    Font *font;

    World *world;

public:
    void Create(int width, int height);
    ~Game();

    /// top-level update method
    bool Update(GameTime);

    /// top-level draw method
    void Draw(Matrix const &);

    /// returns true if the application is intending to close
    bool Finished() const { return finished; }

    /// returns true if all subsystems initialised correctly
    bool Initialised() const { return initialised; }

    /// returns the default font
    Font *GetFont() const { return font; }

    /// returns the game-time structure
    GameTime GetTime() const;

    /// returns the time at the start of this frame
    Time TimeNow() const;

    SDL_Surface *GetSurface() const;
    Color MakeColor(int r, int g, int b) const;

    /// move to a new phase
    void PhaseChange(Phase::Base *next_phase, Time transition_time = 0);

    void SetWorld(World *W) { world = W; }
    World *GetWorld() const { return world; }

    /// return the current phase downcast to a given specific phase if it matches
    template <class T>
    T *GetPhase() const { return InPhase<T>() ? static_cast<T *>(GetPhaseBase()) : 0; }

    /// return true if currently in the given phase
    template <class T>
    bool InPhase() const { return GetPhaseBase() && GetPhaseBase()->IsType<T>(); }

    /// return the current game phase base
    Phase::Base *GetPhaseBase() const { return phase; }

protected:
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

    /// prepare the SDL system
    bool InitialiseSDL(int width, int height);
};

