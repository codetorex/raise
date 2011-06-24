#ifndef TENCODING_H
#define TENCODING_H

#include "tencodinginfo.h"

class UTF8Encoding;

class TEncoding: public TEncodingInfo
{
public:
	static UTF8Encoding* UTF8;

	static byte* Convert(TEncoding* srcEncoding, TEncoding* dstEncoding, byte* src,int srcLength);

public:
	/**
	 * @brief Gets maximum byte count required when a string encoded.
	 * @param charCount Number of characters.
	 * @return The maximum byte count.
	 */
	virtual int GetMaxByteCount(int charCount) = 0;


	/**
	 * @brief Gets a maximum character count required when a string decoded from byte array.
	 * @param byteCount Number of bytes.
	 * @return The maximum character count.
	 */
	virtual int GetMaxCharCount(int byteCount) = 0;

	/**
	 * @brief Gets necessary byte count when a string is encoded in this encoding.
	 * @param data The character array.
	 * @param length The length of character array.
	 * @return The byte count.
	 */
	virtual int GetByteCount(const ch32* src,int length) = 0;


	/**
	 * @brief Gets character count from an encoded byte array.
	 * @param data Pointer to byte array.
	 * @param length The length of byte array.
	 * @return The character count.
	 */
	virtual int GetCharCount(const byte* src, int capacity) = 0;

	

	/**
	 * @brief Gets char and byte count of a encoded array.
	 * @param data Pointer to byte array.
	 * @param [out] length The length of string.
	 * @param [out] bytelength The byte length of string.
	 */
	virtual void Length(const byte* src,int capacity, dword& length, dword& bytelength) = 0;

	/**
	 * @brief Decodes a character
	 * @param data Pointer to encoded byte array.
	 * @param [out] byteLength Decoded byte count.
	 * @return The character.
	 */
	virtual ch32 GetChar(const byte* src,int& byteLength) = 0;


	/**
	 * @brief Gets a character and advances pointer.
	 * @param [in] src source byte array.
	 * @return The character.
	 */
	virtual inline ch32 GetCharAdv( byte*& src );

	/**
	 * @brief Encodes a char into a byte array.
	 * @param [out] dst Destination byte array.
	 * @param character The character.
	 * @return Number of bytes written to dst array.
	 */
	virtual int GetBytes(byte* dst, ch32 character) = 0;


	/**
	 * @brief Encodes a char into a byte array and advances pointer.
	 * @param [out] dst Destination byte array.
	 * @param character The character.
	 */
	virtual inline void GetBytesAdv(byte*& dst, ch32 character);
};

ch32 TEncoding::GetCharAdv( byte*& src )
{
	int ln;
	ch32 result = GetChar(src,ln);
	src += ln;
	return result;
}

void TEncoding::GetBytesAdv( byte*& dst, ch32 character )
{
	int ln = GetBytes(dst,character);
	dst += ln;
}

typedef TEncoding Encoding;

#endif