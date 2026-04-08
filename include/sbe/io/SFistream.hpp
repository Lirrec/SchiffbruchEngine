#ifndef SFISTREAM_H
#define SFISTREAM_H

#include <SFML/System/InputStream.hpp>

#include <cstddef>
#include <istream>
#include <optional>

namespace sbe
{
	/**
		This class is an implementation of sf::Inputstream on std::istreams.
		@warning the getSize() method requires seeking to the end of the stream, which can be very inefficient or time consuming in some cases
	*/
	class sfIStream : public sf::InputStream
	{
	public :

		/// constructor, takes a reference to a std::istream object
		sfIStream(std::istream& i)
				: stream(i) { }

		virtual ~sfIStream() { }

		/// @see sf::InputStream Documentation
		[[nodiscard]] std::optional<std::size_t> read(void* data, std::size_t size) override;

		/// @see sf::InputStream Documentation
		[[nodiscard]] std::optional<std::size_t> seek(std::size_t position) override;

		/// @see sf::InputStream Documentation
		[[nodiscard]] std::optional<std::size_t> tell() override;

		/// @see sf::InputStream Documentation
		std::optional<std::size_t> getSize() override;

	private :
		std::istream& stream;

	};

} // namespace sbe
#endif // SFISTREAM_H
