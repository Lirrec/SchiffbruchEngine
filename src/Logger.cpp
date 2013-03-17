#include <boost/thread/mutex.hpp>
#include "sbe/Logger.hpp"

namespace sbe
{
	boost::mutex Logger::io_mutex;
} // namespace sbe
