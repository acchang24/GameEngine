#include "JobManager.h"
#include <iostream>

JobManager::JobManager() :
    mIsRunning(false),
    mNumJobs(0),
    mNumThreads(std::thread::hardware_concurrency() / 2) // default to half the number of threads available on cpu
{
    // Ensure at least 1 worker thread on lower end systems
    if (mNumThreads == 0)
    {
        mNumThreads = 1;
    }

    // Reserve the amount of threads available from the cpu
    mThreads.reserve(mNumThreads);
}

JobManager::~JobManager()
{
    // End here in case End() wasn't called
    if (mIsRunning)
    {
        End();
    }

    std::cout << "Delete JobManager\n";
}

void JobManager::Begin()
{
    mIsRunning = true;

    for (size_t i = 0; i < mNumThreads; ++i)
    {
        mThreads.emplace_back(&JobManager::WorkerThread, this);
    }
}

void JobManager::End()
{
    std::cout << "Ending JobManager\n";

    {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mIsRunning = false;
    }

    // Wake up all threads so they notice mRunning is false
    mCondition.notify_all();

    // Join each thread
    for (auto& thread : mThreads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    mThreads.clear();

    // Clear the job queue just in case

    std::lock_guard<std::mutex> lock(mQueueMutex);
    while (!mJobQueue.empty())
    {
        Job* leftOverJob = mJobQueue.front();
        mJobQueue.pop();

        if (leftOverJob)
        {
            // Check if the job deletes itself here
            if (leftOverJob->mAutoDelete)
            {
                delete leftOverJob;
            }
        }
    }

    mNumJobs = 0;
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

    // Main thread safely block here until all jobs hit true 0
    mIdleCondition.wait(lock, [this]() {
        return mJobQueue.empty() && mNumJobs == 0;
    });
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

            // Safe Predicate Wait: Handles spurious wakeups and shutdown
            mCondition.wait(lock, [this]() {
                return !mJobQueue.empty() || !mIsRunning;
            });

            // If the engine is stopping and the queue is clear, exit the thread loop!
            if (!mIsRunning && mJobQueue.empty()) 
            {
                return;
            }

            job = mJobQueue.front();
            mJobQueue.pop();

            // Mutex auto unlocks here
        }

        // Execute the job outside of the mutex lock
        if (job)
        {
            job->DoJob();

            if (job->mAutoDelete)
            {
                delete job;
            }

            {
                // Lock mutex in new scope
                std::lock_guard<std::mutex> lock(mQueueMutex);

                // Decrement job count after the work is finished
                --mNumJobs;

                // If last job remaning, notify the main thread
                if (mJobQueue.empty() && mNumJobs == 0)
                {
                    mIdleCondition.notify_all();
                }
                // Mutex auto unlocks here
            }
        }
    }
}
