#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class JobManager
{
public:
    class Job
	{
	public:
		// Abstract function to do a job
		virtual void DoJob() = 0;
        virtual ~Job() = default;
	private:
	};

    // Get the instance of the job manager
    static JobManager* Get();

    // Create and start worker threads running JobManager::WorkerThread()
    void Begin();

    // Stop all threads and clean up
    void End();

    // Add a new job to the queue
    void AddJob(Job* job);

    // Block and wait until all jobs are completed
    void WaitForJobs();

private:
    JobManager();
    ~JobManager();

    // Thread loop function that waits for a job in the queue. 
    // If there is one, it will execute that job and wait for the next one
    // Each thread will run this while loop until the JobManager object is destroyed
    void WorkerThread();

    // Array of threads used for jobs
    std::vector<std::thread> mThreads;
    
    // Queue of jobs (shared resource for all threads)
    std::queue<Job*> mJobQueue;

    // Mutex to protect access to the job queue across all threads
    std::mutex mQueueMutex;

    // Condition used to wake up worker threads when a new job is added
    std::condition_variable mCondition;

    // Condition used by the main thread to wait until all the jobs are completed
    std::condition_variable mIdleCondition;

    // Bool for when the job manager is running
    std::atomic<bool> mIsRunning;

    // Number of jobs in the queue
    unsigned int mNumJobs;

    // Number of threads
    unsigned int mNumThreads;
};
