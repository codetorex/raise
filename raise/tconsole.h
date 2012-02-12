#ifndef TCONSOLE_H
#define TCONSOLE_H

#include "tfilestream.h"

// FOR LINUX IMPLEMENTATION USE CURSES LIBRARY

class TConsole
{
public:

	enum ConsoleColor
	{
		CC_BLACK = 0,
		CC_BLUE = 1,
		CC_GREEN = 2,
		CC_AQUA = 3,
		CC_RED = 4,
		CC_PURPLE = 5,
		CC_YELLOW = 6,
		CC_WHITE = 7,
		CC_GRAY = 8,
		CC_LIGHTBLUE = 9,
		CC_LIGHTGREEN = 10,
		CC_LIGHTAQUA = 11,
		CC_LIGHTRED = 12,
		CC_LIGHTPURPLE = 13,
		CC_LIGHTYELLOW = 14,
		CC_BRIGHTWHITE = 15,
	};

	inline TStream* OpenStandartOutput()
	{
		return new TFileStream(stdout,"stdout",fm_Write);
	}

	inline TStream* OpenStandartInput()
	{
		return new TFileStream(stdin,"stdin",fm_Read);
	}
	
	inline TStream* OpenStandartError()
	{
		return new TFileStream(stderr,"stderr",fm_Write);
	}


	int CurrentBackColor;
	int CurrentForeColor;

	int CursorX;
	int CursorY;

#ifdef WIN32



	HANDLE ConsoleHandle;

	inline void Initialize()
	{
		ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	inline void GetPosition()
	{
		CONSOLE_SCREEN_BUFFER_INFO bi;
		GetConsoleScreenBufferInfo(ConsoleHandle, &bi);
		
		CursorX = bi.dwCursorPosition.X;
		CursorY = bi.dwCursorPosition.Y;
	}

	inline void SetPosition(int x, int y)
	{
		COORD spos;
		spos.X = x;
		spos.Y = y;
		SetConsoleCursorPosition(ConsoleHandle,spos);
	}

	inline void SetColor(ConsoleColor BackColor, ConsoleColor ForeColor)
	{
		ui16 resultColor = ((int)BackColor << 4) | (int)ForeColor;
		SetConsoleTextAttribute(ConsoleHandle, resultColor);
	}

	inline void Write(const TString& value)
	{
		if (value.IsASCII())
		{
			//puts((char*)value.Data);
			fwrite(value.Data,1,value.ByteLength,stdout);
		}
		else
		{
			throw Exception("Unsupported string format for console output");
		}
	}

	inline int ReadKey(bool intercept = true)
	{
		return getchar();
	}

	inline void WriteLine()
	{
		printf("\n");
	}

#else

	inline void Initialize()
	{
		
	}

	inline void GetPosition()
	{
		
	}

	inline void SetPosition(int x, int y)
	{
		
	}

	inline void SetColor(ConsoleColor BackColor, ConsoleColor ForeColor)
	{
		
	}

	inline void Write(const TString& value)
	{
		
	}

	inline void WriteLine()
	{
		
	}


#endif

	
	inline void WriteLine(const TString& value)
	{
		Write(value);
		WriteLine();
	}

	/**
	 * Writes a string but doesn't moves the caret.
	 */
	inline void Insert(const TString& value)
	{
		GetPosition();
		Write(value);
		SetPosition(CursorX,CursorY);
	}
};

extern TConsole Console;

#endif