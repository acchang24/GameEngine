#include "JobManager.h"
#include <iostream>

JobManager* JobManager::Get()
{
	static JobManager s_JobManager;

	return &s_JobManager;
}

void JobManager::Begin()
{
    for (size_t i = 0; i < mNumThreads; ++i)
    {
        mThreads.emplace_back(&JobManager::WorkerThread, this);
    }
}

void JobManager::End()
{
    mIsRunning = false;
    mCondition.notify_all();

    // Join each thread
    for (size_t i = 0; i < mNumThreads; ++i)
    {
        if (mThreads[i].joinable())
        {
            mThreads[i].join();
        }
    }
    mThreads.clear();

    // Clear the job queue just in case
    while (!mJobQueue.empty())
    {
        mJobQueue.pop();
    }
}

void JobManager::AddJob(Job* job)
{
    // MUTEX SCOPE
    {
        // Lock mutex and add a job to the queue
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mJobQueue.push(job);
        ++mNumJobs;
        // Mutex unlocks here
    }
    mCondition.notify_one();
}

void JobManager::WaitForJobs()
{
    std::unique_lock<std::mutex> lock(mQueueMutex);

    while (!(mJobQueue.empty() && mNumJobs == 0))
    {
        mIdleCondition.wait(lock);
    }
}

JobManager::JobManager() :
    mIsRunning(true),
    mNumJobs(0),
    mNumThreads(std::thread::hardware_concurrency() / 2)
{
    // Reserve half the amount of threads available from the cpu
    mThreads.reserve(mNumThreads);
}

JobManager::~JobManager()
{
    std::cout << "Delete JobManager\n";
}

void JobManager::WorkerThread()
{
    while (mIsRunning)
    {
        Job* job = nullptr;
        // MUTEX SCOPE
        {
            // Lock mutex with unique lock
            std::unique_lock<std::mutex> lock(mQueueMutex);
            // If there is a job, pop from queue, if not just wait for a job
            if (mJobQueue.empty())
            {
                mCondition.wait(lock);
            }
            else 
            {
                job = mJobQueue.front();
                mJobQueue.pop();
                --mNumJobs;
            }
            // Mutex auto unlocks here
        }
        if (job)
        {
            job->DoJob();
            {
                std::lock_guard<std::mutex> lock(mQueueMutex);

                if (mJobQueue.empty() && mNumJobs == 0)
                {
                    mIdleCondition.notify_all();
                }
                // Mutex auto unlocks here
            }
        }
    }
}
