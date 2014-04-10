#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <sbe/Engine.hpp>

#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <memory>

#include <boost/any.hpp>

namespace boost {
	class thread_group;
	class barrier;
}

namespace sbe
{

	/// taken from http://stackoverflow.com/questions/14226952/partitioning-batch-chunk-a-container-into-equal-sized-pieces-using-std-algorithm
	template <typename It>
	std::vector<std::pair<It,It>> chunk(It range_from, It range_to, const std::ptrdiff_t num)
	{
		/* Aliases, to make the rest of the code more readable. */
		using std::vector;
		using std::pair;
		using std::make_pair;
		using std::distance;
		using diff_t = std::ptrdiff_t;

		/* Total item number and portion size. */
		const diff_t total
		{
			distance(range_from,range_to)
		};
		const diff_t portion
		{
			total / num
		};

		vector<pair<It,It>> chunks(num);

		It portion_end
		{ range_from };

		/* Use the 'generate' algorithm to create portions. */
		std::generate(begin(chunks),end(chunks),[range_from,&portion_end,portion]()
		{
			It portion_start
			{ portion_end };

			portion_end += portion;
			//Engine::out() << "Chunk from " << std::distance(range_from, portion_start) << " to " << std::distance(range_from, portion_end) << std::endl;
			return make_pair(portion_start,portion_end);
		});

		/* The last portion's end must always be 'range_to'. */
		chunks.back().second = range_to;

		//Engine::out() << " Chunked Vector of size " << total << " to " << num << " chunks of size " << portion << std::endl;

		return chunks;
	}

	class ThreadPool
	{
		public:
			ThreadPool();
			~ThreadPool();

			/// Start num threads to be used by the ThreadPool. Has to be called before starting any job.
			void InitThreads( unsigned int num );
			void StopThreads();

			/**
				Execute a Job J on all elements of vector Objects.
				The vector will partitioned in evenly sized chunks and distributed on all running threads.
				Set the number of required threads with InitThreads() before calling this.
			*/
			template<typename T>
			void runJobOnVector(std::vector<T>& Objects, std::function<void(T&)>& J)
			{
				if (numThreads == 0) {
					Engine::out() << "[ThreadPool] Can't run job, no threads initialized!" << std::endl;
					return;
				}

				typedef typename std::vector<T>::iterator It;
				typedef std::pair<It,It> range_t;

				ranges = chunk(Objects.begin(), Objects.end(), numThreads);

				Job = [this, J]( int tid ) -> void {
					auto r = boost::any_cast<std::vector<range_t>>(ranges);
					//auto end = ++(r[tid].second);
					for (auto it = r[tid].first; it != r[tid].second; ++it)
						J(*it);
						//Engine::out() << it->position << std::endl;
				};

				runJob();
			}


		private:

			void runJob();
			/// thread entry point
			void threadentry(int tid);

			unsigned int numThreads = 0;

			std::function<void(int)> Job;
			std::unique_ptr<boost::thread_group> threads;
			/// contains the param for all threads
			boost::any ranges;
			std::unique_ptr<boost::barrier> startBarrier;
			std::unique_ptr<boost::barrier> endBarrier;
	};


} // namespace sbe



#endif // THREADPOOL_HPP

