#ifndef SFISTREAM_H
#define SFISTREAM_H

#include <SFML/InputStream.hpp>

#include <istream>

class sfistream : public sf::InputStream
{
public :

    sfistream(std::istream& i)
		: stream(i)
	{}

    virtual Int64 read(void* data, Int64 size);
    virtual Int64 seek(Int64 position);
    virtual Int64 tell();
    virtual Int64 getSize() delete;

private :
	std::istream& stream;

};


#endif // SFISTREAM_H
