#ifndef TDECODER_H
#define TDECODER_H

class TEncoder
{
public:
	virtual dword GetChar(byte*& data) = 0;
};

#endif