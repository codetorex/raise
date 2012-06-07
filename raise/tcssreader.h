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
	TArray< TString* > Selectors;
	TDictionary< TString* > Properties;
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

	TArray< TCSSElement* > Elements;

	void Parse(bool closeStream = true);
};


#endif