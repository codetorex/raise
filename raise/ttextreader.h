#ifndef TTEXTREADER_H
#define TTEXTREADER_H

#include "tstring.h"

//TODO: make this class converts its content to UTF-8 to UTF-16

class TTextReader
{
public:
	/// Indicates end reading.
	bool EndOfStream;

	/**
	 * @brief Reads a character without moving to next character.
	 * @return The character.
	 */

	virtual ch32 Peek() = 0;

	/**
	 * @brief Reads single character.
	 * @return Read character.
	 */
	virtual ch32 Read() = 0;

	/**
	 * @brief Reads characters as given amount.
	 * @param [in,out] buffer The buffer and should be not null.
	 * @param count Number of characters to read.
	 * @return Read character count.
	 */
	virtual int Read(ch32* buffer, int count) = 0;

	/**
	 * @brief Reads a line.
	 * @return The line.
	 */
	virtual String ReadLine() = 0;

	/**
	 * @brief Reads to end.
	 * @return String of file.
	 */
	virtual String ReadToEnd() = 0;

	/**
	 * @brief Reads string until encounters with any of interruption character.
	 * @param interrupChars The interruption characters.
	 * @param ignoreChars The ignore characters.
	 * @param [out] interrupt The interrupt.
	 * @return The string before interruption.
	 */
	virtual String ReadUntil(const String& interrupChars, const String& ignoreChars, int& interrupt) = 0;


	virtual String ReadUntilString(const String& matchString) 
	{
		// TODO: change readUntil and use this instead and make it work with ch32 array
		throw NotImplementedException(__FILE__,__LINE__);
		return String::Empty;
	}

	/**
	 * Closes the reader.
	 */
	virtual void Close() = 0;
};

#endif