#include <sbe/io/SFistream.hpp>

#include <sbe/Engine.hpp>

namespace sbe
{

	std::optional<std::size_t> sfIStream::read(void* data, std::size_t size) {
		getSize();
		stream.read((char*) data, (std::streamsize) size);

		if (stream.fail() && !stream.eof())
			return std::nullopt;
		return static_cast<std::size_t>(stream.gcount());
	}

	std::optional<std::size_t> sfIStream::seek(std::size_t position) {
		stream.seekg(static_cast<std::streamoff>(position));
		if (stream.fail())
			return std::nullopt;
		return static_cast<std::size_t>(stream.tellg());
	}

	std::optional<std::size_t> sfIStream::tell() {
		auto re = stream.tellg();
		if (stream.fail())
			return std::nullopt;
		return static_cast<std::size_t>(re);
	}

	std::optional<std::size_t> sfIStream::getSize() {
		auto pos = stream.tellg();
		stream.seekg(0, std::ios::end);
		auto uiLength = stream.tellg();
		stream.seekg(pos);

		if (stream.fail())
			return std::nullopt;
		return static_cast<std::size_t>(uiLength);
	}

} // namespace sbe
