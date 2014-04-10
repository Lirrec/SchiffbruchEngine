#include <sbe/util/ThreadPool.hpp>

#include <boost/thread.hpp>

namespace sbe
{

	ThreadPool::ThreadPool()
	{
		threads.reset ( new boost::thread_group );
	}

	ThreadPool::~ThreadPool()
	{
		StopThreads();
	}


	void ThreadPool::InitThreads( unsigned int num )
	{
		if ( numThreads != 0 && numThreads != num)
			StopThreads();

		if ( numThreads == num ) return;

		numThreads = num;

		startBarrier.reset( new boost::barrier( numThreads+1 ));
		endBarrier.reset  ( new boost::barrier( numThreads+1 ));

		for ( int thread = 0; thread < numThreads; ++thread)
			threads->add_thread( new boost::thread( std::bind( &ThreadPool::threadentry, this, thread )));
	}

	void ThreadPool::StopThreads()
	{
		threads->interrupt_all();
		threads->join_all();
		numThreads = 0;
	}

	/// thread entry point
	void ThreadPool::threadentry(int tid)
	{
		while ( !boost::this_thread::interruption_requested() )
		{
			startBarrier->wait();
			Job(tid);
			endBarrier->wait();
		}
	}

	void ThreadPool::runJob()
	{
		startBarrier->wait();
		endBarrier->wait();
	}

} // namespace sbe



