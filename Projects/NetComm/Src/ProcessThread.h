#pragma once

#include "ThreadSafeMessaging.h"

namespace boost
{
class thread;
};
///
/// Abstract ProcessThread, used as a "safer" thread.
///
class ProcessThread : public ThreadSafeMessaging
{
public:
	///-----------------------------------------------------------------------------------
	/// Assigns a unique id to the created ProcessThread.
	/// \return 
	///-----------------------------------------------------------------------------------
	ProcessThread();

	virtual ~ProcessThread();
	
	///-----------------------------------------------------------------------------------
	/// Starts a new thread running the overridden "body" method.
	/// \return int (0 if OK)
	///-----------------------------------------------------------------------------------
	int start();

	///-----------------------------------------------------------------------------------
	/// Tries to stop the thread by sending a "shutdown" message to
	/// the ProcessThread. Joins (blocks) the thread until it finishes.
	/// \return void
	///-----------------------------------------------------------------------------------
	void stop();

	///-----------------------------------------------------------------------------------
	/// Calls Stop() and Start() for convenience.
	/// \return void
	///-----------------------------------------------------------------------------------
	void restart();

	///-----------------------------------------------------------------------------------
	/// Get the ProcessThread's unique id.
	/// \return int
	///-----------------------------------------------------------------------------------
	int getId() const;

	///-----------------------------------------------------------------------------------
	/// Calls the running thread's internal interrupt method. Will most likely give
	/// unwanted results!
	/// \return void
	///-----------------------------------------------------------------------------------
	void interrupt();

	///-----------------------------------------------------------------------------------
	/// Override the body method in the making of a ProcessThread.
	/// \return void
	///-----------------------------------------------------------------------------------
	virtual void body() = 0;

private:
	int m_id; ///< Unique id.
	boost::thread* m_thread; ///< Thread that will run the super class' body() method.

private:
	static int s_nextId; ///< Manages the unique id's given to ProcessThreads.

};