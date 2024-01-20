#include "JobManager.h"
#include <iostream>
#include <queue>
#include <mutex>
#include <atomic>

// Shutdown signal to tell worker threads to quit
static volatile bool s_Shutdown = false;
// Queue for jobs
static std::queue<JobManager::Job*> s_JobQ;
// Number of jobs
static std::atomic<int> s_JobCount = 0;

// Mutex to ensure only one thread can access the job queue at a time
static std::mutex* GetMutex()
{
	static std::mutex s_JobQueueMutex;
	return &s_JobQueueMutex;
}

void JobManager::Begin()
{
	for (int i = 0; i < NUM_WORKER; ++i)
	{
		mWorkers[i].Begin();
	}
}

void JobManager::End()
{
	s_Shutdown = true;
	for (int i = 0; i < NUM_WORKER; ++i)
	{
		mWorkers[i].End();
	}
}

void JobManager::AddJob(Job* job)
{
	GetMutex()->lock();
	s_JobQ.push(job);
	++s_JobCount;
	GetMutex()->unlock();
}

void JobManager::WaitForJobs()
{
	while (s_JobCount > 0)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}

void JobManager::Worker::Begin()
{
	mThread = new std::thread(Loop);
}

void JobManager::Worker::End()
{
	std::cout << "Delete thread" << std::endl;
	mThread->join();
	delete mThread;
}

void JobManager::Worker::Loop()
{
	while (s_Shutdown == false)
	{
		GetMutex()->lock();
		if (s_JobQ.empty())
		{
			GetMutex()->unlock();
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
		else
		{
			Job* job = s_JobQ.front();
			s_JobQ.pop();
			GetMutex()->unlock();
			job->DoIt();
			--s_JobCount;
		}
	}
}
