// (C) 2009 christian.schladetsch@gmail.com

#pragma once

struct Level;

// the different phases of the game are represented as different types
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

    struct Base : Object
    {
        virtual bool InputEvent(SDL_Event const &) { return false; }

        virtual void Enter(Base * /*previous*/) {  }

        virtual void Leave(Base * /*next*/) {  Delete(); }
    };

    struct Boot : Base
    {
        void Prepare();
        bool InputEvent(SDL_Event const &);
        bool Update(GameTime);
        void Draw(Matrix const &);
    };

    struct Attract : Base
    {
        void Prepare();
        bool Update(GameTime);
        void Draw(Matrix const &);
    };

    struct Play : Base
    {
        World *world = nullptr;
        int level = 0;

        Play();
        ~Play();

        // overrides for Object
        void Prepare();
        void SetLevel(int level);
        bool Update(GameTime);
        void Draw(Matrix const &);

        // overrides for Phase
        bool InputEvent(SDL_Event const &);
        void Leave(Base  * /*next*/) { /* don't delete when leaving - GameOver phase will do that */ }

    protected:
        void PlayerDirects(Direction);
        void PlayerUnDirects(Direction);
    };

    // animation for moving from one level to another
    struct Transition : Base
    {
        void Prepare();
        bool Update(GameTime);
        void Draw();
    };

    // show the end score, high-scores and allow player to enter name
    struct GameOver : Base
    {
        int score;
        Base *prev;

        void Prepare();
        void Enter(Base *previous);
        bool InputEvent(SDL_Event const &);
        bool Update(GameTime);
        void Draw(Matrix const &);
        void Leave(Base *next);
    };
}

