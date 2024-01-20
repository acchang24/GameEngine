#pragma once
#include <string>
#include <unordered_map>
#include <chrono>
#include <iostream>

// Macro to create a scoped timer every time PROFILE_SCOPE() is called
#define PROFILE_SCOPE(name) \
Profiler::ScopedTimer name##_scope(Profiler::Get()->GetTimer(std::string(#name)))

// Profiler class is able to measure how long things take
class Profiler
{
public:
	// Timer class acts like a stopwatch
	class Timer
	{
		friend class Profiler;
	public:
		// Starts recording the time
		void Start();

		// Stops the timer and calculates the duration
		void Stop();

		// Adds the total for this frame to the overall total and
		// increases the number of frames count. It then updates the
		// longest frame time and resets the current frame's duration to 0
		void Reset();

		// Gets the name of the timer
		// @return - const std::string& for the name of the timer
		const std::string& GetName() const { return mName; }

		// Gets the latest frame's total in milliseconds
		// @return - double for the current frame's time
		double GetTimeMs() const { return mCurrentMs; }

		// Gets the longest frame's total in milliseconds
		// @return - double for the longest frame's time
		double GetMaxMs() const { return mMaxMs; }

		// Calculates the average time
		double GetAverageMs() const 
		{
			if (mNumFrames > 0)
			{
				return mTotalTime / mNumFrames;
			}
			return 0.0;
		}

	private:
		// Timer constructor
		// @param - const std::string& for the name of the timer
		Timer(const std::string& name) :
			mName(name),
			mCurrentMs(0.0),
			mMaxMs(0.0),
			mTotalTime(0.0),
			mNumFrames(0)
		{}
		~Timer() {}

		// Name of the timer
		std::string mName;
		// How long this timer has taken in this frame in milliseconds
		double mCurrentMs;
		// How long this timer was in the longest frame in milliseconds
		double mMaxMs;
		// Total time for this timer for all frames combined
		double mTotalTime;
		// How many frames this timer has been captured for this timer
		int mNumFrames;
		// Time of when this timer started
		std::chrono::high_resolution_clock::time_point mStartTime;
	};

	// Scoped timer is created to determine the time it takes
	// to run code in a scope
	class ScopedTimer
	{
	public:
		// ScopedTimer constructor saves the Timer* and calls Timer::Start() on it
		// @param - Timer* for the timer
		ScopedTimer(Timer* timer) : mTimer(timer)
		{
			mTimer->Start();
		}
		~ScopedTimer()
		{
			mTimer->Stop();
		}
	private:
		// The timer used for the scope
		Timer* mTimer;
	};

	// Creates a ScopedTimer given a name (alternative to PROFILE_SCOPE macro)
	ScopedTimer ProfileScope(const std::string& name) 
	{
		Profiler::ScopedTimer timer = Profiler::ScopedTimer(Profiler::Get()->GetTimer(name));
		return timer;
	}

	// Returns the instance of a profiler
	// @return - Profiler* for the static instance of a profiler
	static Profiler* Get();

	// Returns a timer by name if it is in the map. If not,
	// create a timer, add it to the timer map and return it.
	// @return - Timer* for the timer by name
	Timer* GetTimer(const std::string& name);

	// Loops through all the timers and calls Timer::Reset on them
	void ResetAll();

private:
	Profiler();
	// Profiler destructor outputs timer info to a .txt file
	// and calls deletes any timers in the map of timers
	~Profiler();

	std::unordered_map<std::string, Timer*> mTimers;
};