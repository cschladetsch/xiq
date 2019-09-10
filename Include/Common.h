// (C) 2009 christian.schladetsch@gmail.com

/// @file Common includes for all source files

#pragma once

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>

#include <math.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_audio.h>

#ifdef XIQ_HAS_BOOST
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#endif

#ifdef MSVC
#pragma warning(disable:4244)
#endif

typedef unsigned char Unsigned8;
typedef unsigned short Unsigned16;

typedef signed char Signed8;
typedef signed short Signed16;

typedef int TypeNumber;
typedef int Handle;

#include "./Clamp.h"
#include "./ColorValue.h"
#include "./Geometry.h"
#include "./Direction.h"
#include "./GameTime.h"
#include "./Sinusoid.h"
#include "./Random.h"
#include "./Traits.h"

struct ObjectBase;
struct ClassBase;
struct Object;
struct Factory;

struct Xiq;
struct Styx;
struct Player;
struct Impact;
struct Playfield;
struct Level;
struct World;
struct Game;

struct Font;

namespace Phase
{
    struct Boot;
    struct Play;
    struct Attract;
    struct Transition;
    struct GameOver;
};

struct TypeNumbers
{
    enum Value
    {
        Player,
        Xiq,
        Styx,
        Impact,
        Playfield,
        Level,
        World,
        Game,

        PhaseBoot,
        PhaseAttract,
        PhasePlay,
        PhaseTransition,
        PhaseGameOver,
    };
};

#define XIQ_TRAITS(N) \
    CGL_TRAITS(N, TypeNumbers:: N);

XIQ_TRAITS(Player);
XIQ_TRAITS(Xiq);
XIQ_TRAITS(Styx);
XIQ_TRAITS(Impact);
XIQ_TRAITS(Playfield);
XIQ_TRAITS(Level);
XIQ_TRAITS(World);
XIQ_TRAITS(Game);

CGL_TRAITS(Phase::Boot, TypeNumbers::PhaseBoot);
CGL_TRAITS(Phase::Attract, TypeNumbers::PhaseAttract);
CGL_TRAITS(Phase::Play, TypeNumbers::PhasePlay);
CGL_TRAITS(Phase::Transition, TypeNumbers::PhaseTransition);
CGL_TRAITS(Phase::GameOver, TypeNumbers::PhaseGameOver);

#include "Factory.h"
#include "Object.h"


