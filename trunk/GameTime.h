// (C) 2009 www.christian.schladetsch.net

#ifndef GAMETIME_H_INCLUDED
#define GAMETIME_H_INCLUDED

typedef float Time, DeltaTime;

/// GameTime stores time information:
struct GameTime
{
	Time start_game;	///< when the game started
	Time start_frame;	///< when current frame started
	Time total;			///< milliseconds since start of game
	DeltaTime delta;	///< milliseconds since last frame
};

#endif // GAMETIME_H_INCLUDED

//EOF
