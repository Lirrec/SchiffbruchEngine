#include <boost/thread/mutex.hpp>
#include "Logger.h"

boost::mutex Logger::io_mutex;
