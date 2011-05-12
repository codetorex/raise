#ifndef TUTF8ENCODING_H
#define TUTF8ENCODING_H

#include "tencoding.h"

class UTF8Encoding: public TEncoding
{
public:
	int GetByteCount(const byte* data,int length);
	inline int GetCharCount(const byte* data, int capacity);
	void Length(const byte* data, dword& length, dword& bytelength);
	inline ch32 GetChar(byte*& data);
	void GetBytes(byte*& dst, int& capacity, ch32 character);

	ch32 Decode(const byte* charptr);
};

int UTF8Encoding::GetCharCount( const byte* data, int capacity )
{
	dword leng,byteleng;
	Length(data,leng,byteleng);
	return leng;
}

ch32 UTF8Encoding::GetChar( byte*& data )
{

}

#endif