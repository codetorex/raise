#ifndef TCONSOLEFORMATS_H
#define TCONSOLEFORMATS_H

#include "tcompositionmanager.h"

/**
 * These format types will be used with buffer class and terminal emulation.
 */
class TConsoleFormats: public TCompositionManager
{
public:
	TCompositionPrimitive pBackColor32;
	TCompositionPrimitive pForeColor32;
	TCompositionPrimitive pBackColor8;
	TCompositionPrimitive pForeColor8;
	TCompositionPrimitive pAttribute8;
	TCompositionPrimitive pAttribute32;
	TCompositionPrimitive pAttributeColor16;
	TCompositionPrimitive pCharacter8;
	TCompositionPrimitive pCharacter32;

	TBufferFormat* ANSI;
	TBufferFormat* TrueColorUnicode;
	TBufferFormat* TrueColorUnicodeAttributes; // highest possible quality but 16 bytes per char wtf 100*300*16 = 500kb of memory on average

	TConsoleFormats()
	{
		SetPrimitive(pBackColor32,"Back color 32bit ARGB","BG32", tc_dword);
		SetPrimitive(pForeColor32,"Fore color 32bit ARGB","FR32", tc_dword);
		SetPrimitive(pBackColor8,"Back color 8bit Palette","BG8", tc_byte);
		SetPrimitive(pForeColor8,"Fore color 8bit Palette","FR8", tc_byte);

		SetPrimitive(pAttribute8,"8 bit attributes","AT8", tc_byte);
		SetPrimitive(pAttribute8,"32 bit attributes","AT32", tc_dword);
		SetPrimitive(pAttributeColor16, "16 bit color and attributes", "ATC16", tc_short);
		
		SetPrimitive(pCharacter8,"8 bit ASCII character","CH8", tc_byte);
		SetPrimitive(pCharacter32,"32 bit unicode character","CH32", tc_dword);


		ANSI = CreateFormat("ANSI", "A8C8","AT8CH8");
		TrueColorUnicode = CreateFormat("TrueColor Unicode", "C32T32","BG32FG32CH32");
		TrueColorUnicodeAttributes = CreateFormat("TrueColor Unicode Attributes","C32T32A32","BG32FG32AT32CH32");
	}
};

extern TConsoleFormats* ConsoleFormats;

#endif