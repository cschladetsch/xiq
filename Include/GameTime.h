// (C) 2009 christian.schladetsch@gmail.com

#pragma once

// TODO: deal with this cross platform
// ...also need to deal with CPU afinity...
namespace Platform
{
#ifdef LINUX
#include <sys/time.h>
    typedef timeval TimeValue;
#else
    typedef __int64 TimeValue;
#endif
}

typedef double Time, DeltaTime, TimeDelta;

/// stores timing information for update frames
struct GameTime
{
private:
    Platform::TimeValue first;                    ///< when this object was created
    Platform::TimeValue prev_frame_start;

    Time start_frame_millis;    ///< when current frame started
    Time end_frame_millis;        ///< when current frame started
    Time total_millis;            ///< seconds since start of game
    DeltaTime delta_millis;        ///< milliseconds since last frame

    bool use_fixed_step;        ///< if true, use a fixed time-step
    DeltaTime fixed_step_millis;

public:
    GameTime();

    /// call this to start the beginning an update cycle
    void StartFrame();

    /// return the number of seconds that have elapsed since the game started
    Time TotalElapsedSeconds() const { return total_millis/1000.0f; }
    Time Now() const { return TotalElapsedSeconds(); }

    /// return the number of seconds that have elapsed since the game started
    Time TotalElapsedMillis() const { return total_millis; }

    /// the number of milliseconds in the current game udpate
    TimeDelta DeltaMillis() const { return delta_millis; }

    /// the number of seconds in the current game update
    TimeDelta DeltaSeconds() const { return delta_millis/1000.0f; }

    /// fix the reported deltas to a given time interval
    void UseFixedStep(float step)
    {
        use_fixed_step = true;
        fixed_step_millis = step;
    }

    /// return true if the given time is earlier than the total time
    bool IsBefore(Time T) const
    {
        return TotalElapsedSeconds() < T;
    }

    /// return true if the given time is equal to or later than the total time
    bool IsAfter(Time T) const
    {
        return TotalElapsedSeconds() >= T;
    }

private:
    /// get the number of seconds since this object was created
    Platform::TimeValue TakeSample() const;
};

