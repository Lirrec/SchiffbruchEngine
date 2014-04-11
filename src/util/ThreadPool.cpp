#include <sbe/util/ThreadPool.hpp>

#include <boost/thread.hpp>

namespace sbe
{
	std::vector<std::pair<std::ptrdiff_t,std::ptrdiff_t>> chunkInts(const unsigned int total, const std::ptrdiff_t threads)
	{
		/* Aliases, to make the rest of the code more readable. */
		using std::vector;
		using std::pair;
		using std::make_pair;
		using std::distance;
		using diff_t = std::ptrdiff_t;

		/* Total item number and portion size. */
		const diff_t portion = total / threads;

		vector<pair<ptrdiff_t,ptrdiff_t>> chunks(threads);

		diff_t portion_end = 0;

		/* Use the 'generate' algorithm to create portions. */
		std::generate(begin(chunks),end(chunks),[&portion_end,portion]()
		{
			 diff_t portion_start = portion_end;

			portion_end += portion;
			//Engine::out() << " Chunk " << portion_start << " - " << portion_end << std::endl;
			return make_pair(portion_start,portion_end);
		});

		/* The last portion's end must always be 'range_to'. */
		chunks.back().second = total;

		//Engine::out() << " Chunked Vector of size " << total << " to " << threads << " chunks of size " << portion << std::endl;

		return chunks;
	}




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
		try {
			threads->interrupt_all();
			threads->join_all();
			numThreads = 0;
		}
		catch ( ... )
		{
			//Engine::out() << "exception: " << e.what()  <<std::endl;
			Engine::out() << "exception: " << std::endl;
		}
	}

	/// thread entry point
	void ThreadPool::threadentry(int tid)
	{
		try {
			while ( !boost::this_thread::interruption_requested() )
			{
				startBarrier->wait();
				Job(tid);
				endBarrier->wait();
			}
		}
		catch ( ... )
		{
			Engine::out() << "Thread " << tid << " interrupted." << std::endl;
			return;
		}
	}

	void ThreadPool::runJob()
	{
		startBarrier->wait();
		endBarrier->wait();
	}

	void ThreadPool::runCustomJob(const boost::any& data)
	{
		if (numThreads == 0) {
			Engine::out() << "[ThreadPool] Can't run job, no threads initialized!" << std::endl;
			return;
		}

		ranges = data;
		runJob();
	}

} // namespace sbe



