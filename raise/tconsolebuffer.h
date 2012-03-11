#ifndef TCONSOLEBUFFER_H
#define TCONSOLEBUFFER_H

#include "tbitmap.h"
#include "tconsoleformats.h"

/// THESE STUFF WERE EXPERIMENTAL
/// WHEN NEEDED IN FUTURE CODE THESE FROM GCONSOLE AND MOVE CODE TO HERE FROM GCONSOLE

/**
 * This classes will be used for terminal emulation.
 */
class TConsoleDisplay: public TBitmap
{
private:
	inline void SetDataPtrByPos()
	{
		Data = GetPixel(Cursor.X,Cursor.Y);
	}

public:
	TRange ScreenSize;

	/// Position in buffer
	TPosition Cursor;

	/// Screen last line index. Rendering will be done backwards to head.
	int Head;

	byte* WritePos;

	void InitializeBuffer(ui32 _BufferWidth,ui32 _BufferHeight, ui32 _ScreenWidth, ui32 _ScreenHeight, TBufferFormat* _format)
	{
		Create(_BufferWidth,_BufferHeight,_format);
		Head = 0;
		Cursor.SetPosition(0,0);
		ScreenSize.SetRange(_ScreenWidth,_ScreenHeight);
		WritePos = Data;
	}

	void NewLine()
	{
		Head++;
		if (Head >= Height)
		{
			Head = 0;
		}

		Cursor.SetPosition(0,Head);
		SetDataPtrByPos();
	}

	virtual void WriteCharacter(ch32 chr) = 0;
};

/**
 * BG32FG32AT32CH32 formatted console buffer writer
 */
class TConsoleDisplayTrueColor: public TConsoleDisplay
{
public:
	TColor32 BackColor;
	TColor32 ForeColor;
	ui32 Attributes;

	ui32 SavedAttributes;

	void InitializeTrueColorBuffer(ui32 _BufferWidth,ui32 _BufferHeight, ui32 _ScreenWidth, ui32 _ScreenHeight)
	{
		InitializeBuffer(_BufferWidth,_BufferHeight, _ScreenWidth,_ScreenHeight,ConsoleFormats->TrueColorUnicodeAttributes);
	}

	TConsoleDisplayTrueColor()
	{

	}

	void Write(ch32 chr)
	{
		*(ui32*)WritePos = BackColor.color;
		WritePos += 4;
		*(ui32*)WritePos = ForeColor.color;
		WritePos += 4;
		*(ui32*)WritePos = Attributes;
		WritePos += 4;
		*(ui32*)WritePos = chr;
		WritePos += 4;
		Cursor.X ++;
		if (Cursor.X >= Width)
		{
			// actually if its not wrap mode it shouldn't new line but omit the character until new line.
			NewLine();
		}
	}
};

/*enum TConsoleAttributes
{
	CA_BOLD = 1,
	CA_UNDERSCORE = 2,
	CA_BLINK = 4,
};*/

class TConsoleEscapeHandler
{
public:
	virtual bool HandleEscape(TCharacterEnumerator& ce) = 0;
};

class TConsoleANSIEscapeHandler: public TConsoleEscapeHandler
{
public:
	bool HandleEscape(TCharacterEnumerator& ce)
	{

	}
};

class TConsoleVT100EscapeHandler: public TConsoleEscapeHandler
{
public:
	bool HandleEscape(TCharacterEnumerator& ce)
	{

	}
};

/**
 * Supports ANSI Escape sequences
 */
class TConsoleEmulator
{
public:
	TConsoleDisplay* Buffer;
	bool WrapMode;
	bool Escaped;
	TPosition SavedPosition;

	TString EscapeSequence;

	TConsoleEmulator()
	{
		Escaped = false;
		EscapeSequence.Allocate(128);
	}

	/**
	 * Implement emulator logic for new line.
	 */
	virtual void NewLine()
	{
		if (WrapMode)
		{
			Buffer->NewLine();
		}
	}

	void Write(const TString& value)
	{

		TCharacterEnumerator ce(value);
		while(ce.MoveNext())
		{
			if (Escaped)
			{
				EscapeSequence.AppendUnicodeFast(ce.Current);
			}

			if (ce.Current == '\n')
			{
				NewLine();
			}

			if (ce.Current == 0x1B)
			{
				Escaped = true;
				continue;
			}

			Write(ce.Current);
		}
	}
};




#endif