/**
	@file ConcurrentQueue.hpp
	Thread safe queue by Anthony Williams:
	http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
*/

#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H


#include <boost/thread.hpp>
#include <queue>

namespace sbe
{
	/// Thread safe Queue
	template<typename Data>
	class ConcurrentQueue
		{
		private:
			std::queue<Data> the_queue;
			mutable boost::mutex the_mutex;
			boost::condition_variable the_condition_variable;
		public:
			void push(Data const& data)
			{
				boost::mutex::scoped_lock lock(the_mutex);
				the_queue.push(data);
				lock.unlock();
				the_condition_variable.notify_one();
			}

			bool empty() const
			{
				boost::mutex::scoped_lock lock(the_mutex);
				return the_queue.empty();
			}

			bool try_pop(Data& popped_value)
			{
				boost::mutex::scoped_lock lock(the_mutex);
				if(the_queue.empty())
				{
					return false;
				}

				popped_value=the_queue.front();
				the_queue.pop();
				return true;
			}

			void wait_and_pop(Data& popped_value)
			{
				boost::mutex::scoped_lock lock(the_mutex);
				while(the_queue.empty())
				{
					the_condition_variable.wait(lock);
				}

				popped_value=the_queue.front();
				the_queue.pop();
			}

	};
} // namespace sbe
#endif // CONCURRENT_QUEUE_H
