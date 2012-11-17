#ifndef SFISTREAM_H
#define SFISTREAM_H

#include <SFML/Config.hpp> // Int64
#include <SFML/System/InputStream.hpp>

#include <istream>

using sf::Int64; // or better use int64_t?

class sfistream : public sf::InputStream
{
public :

    sfistream(std::istream& i)
		: stream(i)
	{}

    virtual Int64 read(void* data, Int64 size);
    virtual Int64 seek(Int64 position);
    virtual Int64 tell();
    virtual Int64 getSize() {}

private :
	std::istream& stream;

};


#endif // SFISTREAM_H
