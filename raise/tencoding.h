#ifndef TENCODING_H
#define TENCODING_H

#include "tencodinginfo.h"
#include "tcharbuffer.h"
//#include "tbuffer.h"

class UTF8Encoding;
class UTF16Encoding;
class TLatin1Encoding;

class TEncoding: public TEncodingInfo
{
public:
	static UTF8Encoding& UTF8;
	static UTF16Encoding& UTF16;
	static TLatin1Encoding& Latin1;

	static byte* Convert(TEncoding* srcEncoding, TEncoding* dstEncoding, byte* src,int srcLength);

public:
	/**
	 * @brief Gets maximum byte count required when a string encoded.
	 * @param charCount Number of characters.
	 * @return The maximum byte count.
	 */
	virtual int GetMaxByteCount(int charCount) const = 0;


	/**
	 * @brief Gets a maximum character count required when a string decoded from byte array.
	 * @param byteCount Number of bytes.
	 * @return The maximum character count.
	 */
	virtual int GetMaxCharCount(int byteCount) const = 0;

	/**
	 * @brief Gets necessary byte count when a string is encoded in this encoding.
	 * @param data The character array.
	 * @param length The length of character array.
	 * @return The byte count.
	 */
	virtual int GetByteCount(const ch32* src,int length) const = 0;


	/**
	 * @brief Gets character count from an encoded byte array.
	 * @param data Pointer to byte array.
	 * @param length The length of byte array.
	 * @return The character count.
	 */
	virtual int GetCharCount(const byte* src, int capacity) const = 0;

	

	/**
	 * @brief Gets char and byte count of a encoded array.
	 * @param data Pointer to byte array.
	 * @param [out] length The length of string.
	 * @param [out] bytelength The byte length of string.
	 */
	virtual void Length(const byte* src,ui32 capacity, ui32& length, ui32& bytelength) const = 0;

	/**
	 * @brief Decodes a character
	 * @param data Pointer to encoded byte array.
	 * @param [out] byteLength Decoded byte count.
	 * @return The character.
	 */
	virtual ch32 GetChar(const byte* src,int& byteLength) const = 0;


	/**
	 * @brief Gets a character and advances pointer.
	 * @param [in] src source byte array.
	 * @return The character.
	 */
	virtual inline ch32 GetCharAdv( byte*& src ) const;

	/**
	 * @brief Encodes a char into a byte array.
	 * @param [out] dst Destination byte array.
	 * @param character The character.
	 * @return Number of bytes written to dst array.
	 */
	virtual int GetBytes(byte* dst, ch32 character) const = 0;


	/**
	 * @brief Encodes a char into a byte array and advances pointer.
	 * @param [out] dst Destination byte array.
	 * @param character The character.
	 */
	virtual inline void GetBytesAdv(byte*& dst, ch32 character) const;

	/**
	 * @brief Decodes a byte array to character array.
	 * @param bytes Source byte array
	 * @param bytecount Length of byte array
	 * @param chars Destination character array
	 * @param charcapacity Capacity of char array
	 * @return Returns number of bytes written
	 */
	virtual int GetChars( byte* bytes, int bytecount, ch32* chars , int charcapacity ) const = 0;

	//virtual int GetChars( byte* bytes, int bytecount, TCharBuffer& buffer) = 0;

	//virtual int GetChars( TStream* sorce, TCharBuffer& buffer, int charsToRead ) = 0; 

	//virtual TCharBuffer GetChars( byte* bytes, int bytecount ) = 0;

	/**
	 * Returns how many bytes are read. Converted char count can be get from buffer.index
	 */
	virtual ui32 GetChars( byte* bytes, ui32 bytecount, TCharBuffer& buffer ) const = 0;

	/**
	 * @brief Encodes a char array to this encoding
	 * @param chars Source character array
	 * @param charcount Length of character array
	 * @param bytes Destination byte array
	 * @param bytecapacity Capacity of byte array
	 * @return Returns number of characters written
	 */
	virtual int GetBytes( ch32* chars, int charcount, byte* bytes, int bytecapacity ) const = 0;

	//virtual TByteBuffer GetBytes ( ch32* chars, int charcount ) = 0;

	virtual TString GetString(byte* bytes, int bytecount) const = 0;
};

inline ch32 TEncoding::GetCharAdv( byte*& src ) const
{
	int ln;
	ch32 result = GetChar(src,ln);
	src += ln;
	return result;
}

inline void TEncoding::GetBytesAdv( byte*& dst, ch32 character ) const
{
	int ln = GetBytes(dst,character);
	dst += ln;
}

typedef TEncoding Encoding;

#endif