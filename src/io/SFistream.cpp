#include "sbe/io/SFistream.hpp"

#include "sbe/Engine.hpp"

namespace sbe
{

	Int64 sfIStream::read(void* data, Int64 size)
	{
		//Engine::out() << "read[]: pos: " << stream.tellg() <<  " - data " << data << " - (std::streamsize)size: " << size << std::endl;
		getSize();
		stream.read( (char*)data, (std::streamsize)size );
		//Engine::out() << "read[]: gcount: " << stream.gcount() << std::endl;
		//Engine::out() << "read[]: new pos: " << stream.tellg() << std::endl;

		if ( stream.fail() && !stream.eof() )
		{
			//Engine::out() << "read[]: stream fail!" << std::endl;
			return -1;
		}
		return stream.gcount();
	}

	Int64 sfIStream::seek(Int64 position)
	{
//		Engine::out() << "seek[]: pos " << position << std::endl;
		stream.seekg( position );
		if ( stream.fail() )
		{
//			Engine::out() << "seek[]: stream fail!" << std::endl;
			return -1;
		}
		return stream.tellg();
	}

	Int64 sfIStream::tell()
	{
		auto re = stream.tellg();
//		Engine::out() << "tell[]: re " << re << std::endl;
		if ( stream.fail() )
		{
//			Engine::out() << "tell[]: stream fail!" << std::endl;
			return -1;
		}
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


//		Engine::out() << "getSize[]: size " << uiLength << std::endl;

		if ( stream.fail() )
		{
//			Engine::out() << "getSize[]: stream fail!" << std::endl;
			return -1;
		}
		return uiLength;
	}

} // namespace sbe
