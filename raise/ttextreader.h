#ifndef TTEXTREADER_H
#define TTEXTREADER_H

#include "tstring.h"

//TODO: make this class converts its content to UTF-8 to UTF-16

class TTextReader
{
public:
	virtual int Peek() = 0;
	virtual int Read() = 0;
	virtual int Read(ch16* buffer, int count) = 0;
	virtual str8 ReadLine() = 0; // TODO: you can write a default algorithm from read and peek
	virtual str8 ReadToEnd() = 0; // TODO: you can write default algorithm
	virtual void Close() = 0;
};

#endif