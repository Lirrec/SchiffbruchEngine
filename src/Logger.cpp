#include <boost/thread/mutex.hpp>
#include "sbe/Logger.hpp"

namespace sbe
{
	boost::mutex* Logger::io_mutex = nullptr;

	Logger::Logger( std::ostream& _out ) :out(_out) {
			if (io_mutex == nullptr) io_mutex = new boost::mutex;
		}

	void Logger::lock()
	{
		io_mutex->lock();
	}

	void Logger::unlock()
	{
		io_mutex->unlock();
	}


} // namespace sbe
