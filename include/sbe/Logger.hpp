/**
	@file Logger.hpp
	Original Author: Kiran Mohan
*/

#ifndef LOGGER_H
#define LOGGER_H


#include <iostream>
#include <sstream>

namespace boost
{
	class mutex;
}

namespace sbe
{
	/**
	 * A thread safe logger to print to stdout or stderr
	 */
	class Logger
	{
	public:

		Logger(std::ostream& _out);

		virtual ~Logger() { }

		template<typename T>
		Logger& operator<<(T data) {
			/* Takes any data type and stores in a stringstream
			 */
			lock();
			oss << data;
			out << data;
			unlock();
			return *this;
		}

		Logger& operator<<(std::ostream& (* pf)(std::ostream&)) {
			// for stream manipulators
			lock();
			oss << pf;
			out << pf;
			unlock();
			return *this;
		}

		Logger& operator<<(Logger& (* pf)(Logger&)) {
			//applicator - mainly calling the print function;
			return pf(*this);
		}

		std::string GetLog() const {
			return oss.str();
		}

		void ClearCache(int chars = -1) {
			if (chars == -1)
			{
				// clear everything
				oss.str("");
			}
			else
			{
				oss.str(oss.str().erase(0, chars));
			}

			oss.clear();
		}

	private:

		void lock();

		void unlock();

		static boost::mutex* io_mutex;
		std::stringstream oss;
		std::ostream& out;

	};

} // namespace sbe

#endif  /* LOGGER_H */

