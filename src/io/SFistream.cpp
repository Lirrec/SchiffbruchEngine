#include "sbe/io/SFistream.hpp"

Int64 sfIStream::read(void* data, Int64 size)
{
	stream.read( (char*)data, (std::streamsize)size );
}

Int64 sfIStream::seek(Int64 position)
{
	stream.seekg( position );
}

Int64 sfIStream::tell()
{
	return stream.tellg();
}

Int64 sfIStream::getSize()
{
	// get old pos
	auto pos = stream->tellg();
	// seek to the end and remember pos
	stream->seekg(0, ios::end);
	auto uiLength = stream->tellg();
	// set old pos
	stream->seekg(pos);

	return uiLength;
}
