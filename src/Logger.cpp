#include <boost/thread/mutex.hpp>
#include "sbe/Logger.hpp"

boost::mutex Logger::io_mutex;
