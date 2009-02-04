// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
#include "GameTime.h"

GameTime::GameTime()
{
	first = TakeSample();

	start_frame_millis = 0;
	end_frame_millis = 0;
	total_millis = 0;
	delta_millis = 0;

	use_fixed_step = false;
	fixed_step_millis = 0;
}

void GameTime::StartFrame()
{
	total_millis += delta_millis;
	Time now = TakeSample();
	delta_millis = now - start_frame_millis;
	start_frame_millis = now;
}

Time GameTime::TakeSample() const
{
	return SDL_GetTicks();
}

//EOF
