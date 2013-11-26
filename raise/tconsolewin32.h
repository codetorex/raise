#ifndef TCONSOLEWIN32_H
#define TCONSOLEWIN32_H

#include "tconsolebase.h"


#ifdef WIN32


// FOR LINUX IMPLEMENTATION USE CURSES LIBRARY


class TConsoleWin32: public TConsole
{
public:

	static byte ColorTransformTable[16];

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

	HANDLE ConsoleHandle;

	void CreateConsole(const String& title);

	inline void Initialize()
	{
		ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	/************ BEGIN TCONSOLEDRIVER IMPLEMENTATION **************/

	inline void GetSize()
	{
		CONSOLE_SCREEN_BUFFER_INFO bi;
		GetConsoleScreenBufferInfo(ConsoleHandle, &bi);

		Size.Width = bi.dwSize.X;
		Size.Height = bi.dwSize.Y;
	}

	inline void GetPosition()
	{
		CONSOLE_SCREEN_BUFFER_INFO bi;
		GetConsoleScreenBufferInfo(ConsoleHandle, &bi);

		Cursor.X = bi.dwCursorPosition.X;
		Cursor.Y = bi.dwCursorPosition.Y;
	}

	inline void SetPosition(IPosition& pos)
	{
		COORD spos;
		spos.X = pos.X;
		spos.Y = pos.Y;
		SetConsoleCursorPosition(ConsoleHandle,spos);
	}

	inline void UpdateColorAttribute()
	{
		if (CurrentBackColor > 15 || CurrentForeColor > 15)
		{
			throw Exception ("Win console doesn't supports more than 16 colors and fall back is not implemented yet");
		}
		ui16 resultColor = (ColorTransformTable[CurrentBackColor] << 4) | ColorTransformTable[CurrentForeColor];
		SetConsoleTextAttribute(ConsoleHandle, resultColor);
	}

	inline void SetBackColor( byte color )
	{
		CurrentBackColor = color;
		UpdateColorAttribute();
	}

	inline void SetForeColor( byte color )
	{
		CurrentForeColor = color;
		UpdateColorAttribute();
	}


	inline void SetColor( byte backcolor, byte forecolor)
	{
		CurrentBackColor = backcolor;
		CurrentForeColor = forecolor;
		UpdateColorAttribute();
	}

	/************ END TCONSOLEDRIVER IMPLEMENTATION **************/

	inline void WriteChar(ch32 chr)
	{
		if (chr < 0x80)
		{
			byte cbyte = chr;
			fwrite(&cbyte,1,1,stdout);
		}
		else
		{
			throw Exception("Unicode character not supported by windows console?");
		}
	}

	inline void Write(const String& value)
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

	inline void Write(const TStringBuilder& builder)
	{
		if (builder.ByteLength == builder.Length)
		{
			fwrite(builder.GetData(),1,builder.ByteLength,stdout);
		}
		else
		{
			throw Exception("Unsupported string format for console output");
		}
	}
	
	/**
	 * Reads a key.
	 */
	inline ch32 ReadKey()
	{
		return getchar();
	}

	void Clear()
	{
		COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */ 
		BOOL bSuccess;
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
		DWORD dwConSize;                 /* number of character cells in
											the current buffer */ 

		/* get the number of character cells in the current buffer */ 
		bSuccess = GetConsoleScreenBufferInfo( ConsoleHandle, &csbi );
		
		if (!bSuccess)
		{
			throw Exception ( "GetConsoleScreenBufferInfo" );
		}
		
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		/* fill the entire screen with blanks */ 
		bSuccess = FillConsoleOutputCharacter( ConsoleHandle, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
		
		if (!bSuccess)
		{
			throw Exception ("FillConsoleOutputCharacter");
		}


		/* get the current text attribute */ 
		bSuccess = GetConsoleScreenBufferInfo( ConsoleHandle, &csbi );

		if(!bSuccess)
		{
			throw Exception ("ConsoleScreenBufferInfo");
		}

		/* now set the buffer's attributes accordingly */ 
		bSuccess = FillConsoleOutputAttribute( ConsoleHandle, csbi.wAttributes,
		   dwConSize, coordScreen, &cCharsWritten );

		if(!bSuccess)
		{
			throw Exception ("FillConsoleOutputAttribute");
		}

		/* put the cursor at (0, 0) */ 
		bSuccess = SetConsoleCursorPosition( ConsoleHandle, coordScreen );
		if (!bSuccess)
		{
			throw Exception ("SetConsoleCursorPosition");
		}
	}




};

typedef TConsoleWin32 TConsoleCurrent;

#endif

#endif