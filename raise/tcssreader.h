#ifndef TCSSREADER_H
#define TCSSREADER_H

#include "tstring.h"
#include "tdictionary.h"
#include "tstreamreader.h"


/**
 * CSS is not useful for serializing data but good for style stuff
 */

class TCSSElement
{
public:
	Array< String* > Selectors;
	TDictionary< String* > Properties;
};


/**
 * RML reader maybe work with css test it
 */

class TCSSReader
{
public:
	TStream* BaseStream;
	TStreamReader* TextStream;

	TCSSReader(TStream* readStream);

	Array< TCSSElement* > Elements;

	void Parse(bool closeStream = true);
};


#endif