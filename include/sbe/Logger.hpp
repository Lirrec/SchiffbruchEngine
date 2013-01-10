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
        std::cout << data;
        return *this;
    }

    Logger & operator<<(std::ostream& (*pf)(std::ostream&))
    {
        // for stream manipulators
        boost::mutex::scoped_lock io_mutex_lock(io_mutex);
        oss << pf;
        std::cout << pf;
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

	void ClearCache( int chars = -1 )
	{
		if ( chars == -1 ) 
		{
			// clear everything
			oss.str("");
		}
		else
		{
			oss.ignore( chars );
		}

		oss.clear();
	}

private:

    static boost::mutex io_mutex;
    std::stringstream oss;

};

#endif  /* LOGGER_H */

