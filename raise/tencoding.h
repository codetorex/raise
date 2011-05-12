#ifndef TENCODING_H
#define TENCODING_H

#include "tencodinginfo.h"

class TEncoding: public TEncodingInfo
{
public:
	/**
	 * @brief Gets necessary byte count when a string is encoded in this encoding.
	 * @param data The character array.
	 * @param length The length of character array.
	 * @return The byte count.
	 */
	virtual int GetByteCount(const ch32* data,int length) = 0;


	/**
	 * @brief Gets character count from an encoded byte array.
	 * @param data Pointer to byte array.
	 * @param length The length of byte array.
	 * @return The character count.
	 */
	virtual int GetCharCount(const byte* data, int capacity) = 0;

	/**
	 * @brief Gets char and byte count of a encoded array.
	 * @param data Pointer to byte array.
	 * @param [out] length The length of string.
	 * @param [out] bytelength The byte length of string.
	 */
	virtual void Length(const byte* data, dword& length, dword& bytelength) = 0;

	/**
	 * @brief Decodes a character from 
	 * @param [in,out] data [in,out] If non-null, the data.
	 * @return The character.
	 */
	virtual ch32 GetChar(byte*& data) = 0;


	/**
	 * @brief Encodes a char into a byte array and increments its pointer.
	 * @param [in,out] dst Destination byte array.
	 * @param [in,out] capacity The capacity of byte array.
	 * @param character The character to be encoded.
	 */
	virtual void GetBytes(byte*& dst, int& capacity, ch32 character) = 0;
};

#endif