#include <boost/thread/mutex.hpp>
#include "Logger.hpp"

boost::mutex Logger::io_mutex;
