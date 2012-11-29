#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

///
/// Desc Abstract ProcessThread, used as a "safer" thread.
///
class ProcessThread
{
public:
	///-----------------------------------------------------------------------------------
	/// Desc Assigns a unique id to the created ProcessThread.
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessThread();

	virtual ~ProcessThread();
	
	///-----------------------------------------------------------------------------------
	/// Desc Starts a new thread running the overridden "body" method.
	/// \returns int (0 if OK)
	///-----------------------------------------------------------------------------------
	int start();

	///-----------------------------------------------------------------------------------
	/// Desc Tries to stop the thread by sending a "shutdown" message to
	/// the ProcessThread. Joins (blocks) the thread until it finishes.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void stop();

	///-----------------------------------------------------------------------------------
	/// Desc Calls Stop() and Start() for convenience.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void restart();

	///-----------------------------------------------------------------------------------
	/// Desc Get the ProcessThread's unique id.
	/// \returns const int
	///-----------------------------------------------------------------------------------
	const int getId() const;

	///-----------------------------------------------------------------------------------
	/// Desc Calls the running thread's internal interrupt method. Will most likely give
	/// unwanted results!
	/// \returns void
	///-----------------------------------------------------------------------------------
	void interrupt();

	///-----------------------------------------------------------------------------------
	/// Desc Override the body method in the making of a ProcessThread.
	/// \returns void
	///-----------------------------------------------------------------------------------
	virtual void body() = 0;

private:
	int m_id;
	boost::thread* m_thread;

private:
	static int s_nextId; ///< Gives every ProcessThread a unique id.

};