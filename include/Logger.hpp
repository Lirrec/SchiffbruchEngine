/*
 * File:   logger.h
 * Original Author: Kiran Mohan
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <boost/thread.hpp>
#include <iostream>
/**
 * a thread safe logger to print to stdout or stderr
 */
class Logger
{
public:

    Logger() {}
    virtual ~Logger() {}

    template <typename T>
    Logger & operator <<(T data)
    {
        /* Takes any data type and stores in a stringstream
         */
        boost::mutex::scoped_lock io_mutex_lock(io_mutex);
        oss << data;
        return *this;
    }

    Logger & operator<<(std::ostream& (*pf)(std::ostream&))
    {
        // for stream manipulators
        boost::mutex::scoped_lock io_mutex_lock(io_mutex);
        oss << pf;
        return *this;
    }

    Logger & operator<<(Logger & (*pf)(Logger &))
    {
        //applicator - mainly calling the print function;
        return pf(*this);
    }

	std::string GetLog()
	{
		return oss.str();
	}

	void ClearCache()
	{
		oss.str("");
		oss.clear();
	}

private:

    static boost::mutex io_mutex;
    std::ostringstream oss;

};

#endif  /* LOGGER_H */

