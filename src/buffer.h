// simple FIFO ring buffer
// buffer.h
// @author suikale
// @version 170621

#include <stdlib.h>

class Buffer {
    int read;
    int write;
    int max;
    unsigned char *b = nullptr;
public:
    Buffer(int size);
	~Buffer();
    void push(unsigned char input);
    unsigned char pop();
    bool is_empty();
};
