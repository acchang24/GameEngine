#pragma once
#include <thread>

enum 
{ 
	NUM_WORKER = 4 
};

// JobManager is a job multithread system that helps
// queue jobs on different threads. A job manager has 
// an array of workers that has its own thread. Each 
// worker can then check a queue of jobs that can be
// performed on its respective threads. 
class JobManager
{
public:
	// Job to describe something for a thread to do
	class Job
	{
	public:
		// Abstract function to do a job
		virtual void DoIt() = 0;
	private:
	};

	// Worker class describes different threads that can 
	// be used to do a certain job
	class Worker
	{
	public:
		// Create a new thread that runs Worker::Loop()
		void Begin();
		// Joins the thread and deletes the thread
		void End();
		// The worker's thread will loop and check to checks to see if there are any available jobs.
		// It first locks the job queue with the mutex. It will then pop the 
		// next job in the queue if there is one and unlocks with mutex.
		// It then calls Job::DoIt() and decrements the job count. If it is empty
		// the mutex will unlock the queue and then sleeps waiting for the shutdown signal.
		// Returns when the shutdown signal is received.
		static void Loop();

	private:
		// This worker's thread
		std::thread* mThread;
	};

	// Loops through workers array and calls Worker::Begin() on each worker
	void Begin();

	// Sets the shutdown signal and loops through workers array and calls Worker::End() on each worker
	void End();

	// Mutex locks job queue so that 1 thread can add a job to the job queue. 
	// It then increments the job count and the mutex then unlocks the queue.
	// @param - Job* for a new job
	void AddJob(Job* job);

	// Tells this thread to wait until the jobs are finished
	void WaitForJobs();

private:
	// Array of workers
	Worker mWorkers[NUM_WORKER];
};