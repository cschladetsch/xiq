// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "GameTime.h"

GameTime::GameTime()
{
	start_frame_millis = 0;
	end_frame_millis = 0;
	total_millis = 0;
	delta_millis = 0;

	use_fixed_step = false;
	fixed_step_millis = 0;

#ifdef CGL_PLATFORM_UNIX
	gettimeofday(&first, 0);
#endif

	prev_frame_start = first;
	delta_millis = 0;
}

void GameTime::StartFrame()
{
//	total_millis += delta_millis;
//	Time now = TakeSample();
//	delta_millis = now - start_frame_millis;
//	start_frame_millis = now;

#ifdef CGL_PLATFORM_UNIX
	timeval now;
	gettimeofday(&now, 0);

	double delta_seconds = now.tv_sec - prev_frame_start.tv_sec;
	double delta_micro_seconds = now.tv_usec - prev_frame_start.tv_usec;

	delta_millis = delta_seconds*1000. + delta_micro_seconds/1000.;

	if (delta_millis > 500)
		delta_millis = 500;

	total_millis = (now.tv_sec - first.tv_sec)*1000. + (now.tv_usec - first.tv_usec)/1000.;

	prev_frame_start = now;

#else
	delta_millis = 4;//1.0f/60.0f;
	total_millis += delta_millis;
#endif
}

Platform::TimeValue GameTime::TakeSample() const
{
	assert(0);
	return 0;
}

//EOF
