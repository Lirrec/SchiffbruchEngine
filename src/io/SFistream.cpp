#include "sbe/io/SFistream.hpp"

namespace sbe
{

	Int64 sfIStream::read(void* data, Int64 size)
	{
		stream.read( (char*)data, (std::streamsize)size );
		if ( stream.fail() ) return -1;
		return stream.gcount();
	}

	Int64 sfIStream::seek(Int64 position)
	{
		stream.seekg( position );
		if ( stream.fail() ) return -1;
		return stream.tellg();
	}

	Int64 sfIStream::tell()
	{
		auto re = stream.tellg();
		if ( stream.fail() ) return -1;
		return re;
	}

	Int64 sfIStream::getSize()
	{
		// get old pos
		auto pos = stream.tellg();
		// seek to the end and remember pos
		stream.seekg(0, std::ios::end);
		auto uiLength = stream.tellg();
		// set old pos
		stream.seekg(pos);

		if ( stream.fail() ) return -1;
		return uiLength;
	}

} // namespace sbe
