#include "sbe/io/SFistream.hpp"

Int64 sfistream::read(void* data, Int64 size)
{
	stream.read( (char*)data, (std::streamsize)size );
}

Int64 sfistream::seek(Int64 position)
{
	stream.seekg( position );
}

Int64 sfistream::tell()
{
	return stream.tellg();
}

