#include "Profiler.h"
#include <fstream>

Profiler* Profiler::Get()
{
	static Profiler s_Profiler;

	return &s_Profiler;
}

Profiler::Timer* Profiler::GetTimer(const std::string& name)
{
	if (mTimers.find(name) != mTimers.end())
	{
		return mTimers[name];
	}

	Timer* newTimer = new Timer(name);

	mTimers[name] = newTimer;

	return newTimer;
}

void Profiler::ResetAll()
{
	for (auto& t : mTimers)
	{
		t.second->Reset();
	}
}

Profiler::Profiler()
{
}

Profiler::~Profiler()
{
	std::ofstream outFile("profiler.txt");

	outFile << "name: , current (ms), avg (ms), max (ms)\n";

	for (auto& t : mTimers)
	{
		Timer* timer = t.second;

		outFile << timer->GetName() << ": , " << timer->mCurrentMs << ", " << timer->GetAverageMs() << ", " << timer->GetMaxMs() << "" << "\n";

		delete timer;
	}

	outFile.close();
}

void Profiler::Timer::Start()
{
	mStartTime = std::chrono::high_resolution_clock::now();
}

void Profiler::Timer::Stop()
{
	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	mCurrentMs = std::chrono::duration<double, std::milli>(endTime - mStartTime).count();
}

void Profiler::Timer::Reset()
{
	mTotalTime += mCurrentMs;

	++mNumFrames;

	if (mCurrentMs > mMaxMs)
	{
		mMaxMs = mCurrentMs;
	}
}
