#ifndef SFISTREAM_H
#define SFISTREAM_H

#include <SFML/Config.hpp> // Int64
#include <SFML/System/InputStream.hpp>

#include <istream>

namespace sbe
{
	using sf::Int64; // or better use int64_t?


	/**
		This class is an implementation of sf::Inputstream on std::istreams.
		@warning the getSize() method requires seeking to the end of the stream, which can be very inefficient or time consuming in some cases
	*/
	class sfIStream : public sf::InputStream
	{
	public :

		/// constructor, takes a reference to a std::istream object
		sfIStream(std::istream& i)
			: stream(i)
		{}

		virtual ~sfIStream() {}

		/// @see sf::InputStream Documentation
		virtual Int64 read(void* data, Int64 size);
		/// @see sf::InputStream Documentation
		virtual Int64 seek(Int64 position);
		/// @see sf::InputStream Documentation
		virtual Int64 tell();
		/// @see sf::InputStream Documentation
		virtual Int64 getSize();

	private :
		std::istream& stream;

	};

} // namespace sbe
#endif // SFISTREAM_H
