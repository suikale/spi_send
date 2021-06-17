// simple FIFO ring buffer
// buffer.cpp
// @author suikale
// @version 160621

#include "buffer.h"

Buffer::Buffer(int size)  : read{0}, write{0}, max{size - 1} 
{
    b = new unsigned char[size];
}

Buffer::~Buffer()
{
	delete[] b;
}

void Buffer::push(unsigned char input)
{
    // ei tilaa
    if (write - max == read || write == read - 1)
        return;

    b[write] = input;
    write++;

    if (max < write)
        write = 0;

}

// kasvattaa palautuksen jälkeen 
unsigned char Buffer::pop()
{
    unsigned char c = 0xFF; // 1111 1111
    
    if (is_empty())
        return c;

    c = b[read++];

    if (max < read)
        read = 0;

    return c;
}

bool Buffer::is_empty()
{
    return (write == read);
}
