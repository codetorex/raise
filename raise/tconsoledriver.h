#ifndef TCONSOLEDRIVER_H
#define TCONSOLEDRIVER_H

#include "tstring.h"
#include "ttextwriter.h"
#include "tpalette.h"
#include "tevent.h"
#include "mvector2.h"
#include "msize2.h"


// LETS DEFINE THE STANDARTS HERE
// Rules:
//
// 1. Colors will be in Unix xterm 256 scale, so all other devices should be mapped
//	  if they use different color set. Color names are derived from CSS document.
//
// 2. Applications that involving console should be written using TConsoleDriver interface.
//    This means these code can be work on all console platforms.
//    So our console standard is TConsoleDriver
//
// 3. Incoming data from other applications can be interpreted by VT100 input thing. 
//    Which will parse escape codes and call the appropriate function from Driver.
//
// 4. So creating remote applications can be done via TConsoleVT100Output and
//	  this application can run anywhere without change of code.

enum TConsoleColor
{
	CC_BLACK,
	CC_MAROON,
	CC_GREEN,
	CC_OLIVE,
	CC_NAVY,
	CC_PURPLE,
	CC_TEAL,
	CC_SILVER,
	CC_GRAY,
	CC_RED,
	CC_LIME,
	CC_YELLOW,
	CC_BLUE,
	CC_FUCHSIA,
	CC_AQUA,
	CC_WHITE,
};

class TConsoleDriver: public TTextWriter
{
public:
	static TPalette XTermColors;

	bool WrapMode;


	/// Current back color
	byte CurrentBackColor;

	/// Current fore color
	byte CurrentForeColor;

	/// Current cursor position of console
	IPosition Cursor;

	/// Size of console
	ISize Size;

	virtual void SaveState() {};

	virtual void LoadState() {};

	virtual void SetWarpMode( bool warptext )
	{
		WrapMode = warptext;
	}

	virtual void SetBold(bool bold)
	{

	}
	
	virtual void GetSize() = 0;

	/**
	 * Gets current cursor position and updates Cursor member.
	 */
	virtual void GetPosition() = 0;

	/**
	 * Sets current cursor position as in given parameter.
	 */
	virtual void SetPosition(IPosition& pos) = 0;

	/**
	 * Sets current back color.
	 */
	virtual void SetBackColor( byte color ) = 0;

	/**
	 * Sets current fore color.
	 */
	virtual void SetForeColor( byte color ) = 0;

	virtual void Clear() = 0;

	/**
	 * Write char to console.
	 */
	virtual void WriteChar(ch32 chr) = 0;

	/**
	 * Gets current position and writes to given referenced parameter.
	 */
	inline void GetPosition(IPosition& pos)
	{
		GetPosition();
		pos.X = Cursor.X;
		pos.Y = Cursor.Y;
	}

	/**
	 * Sets both back and forecolor same time.
	 */
	inline virtual void SetColor( byte backcolor, byte forecolor)
	{
		SetBackColor(backcolor);
		SetForeColor(forecolor);
	}

	/**
	 * Sets position by simple x and y parameters instead position object.
	 */
	inline void SetPosition(int x,int y)
	{
		IPosition pos(x,y);
		SetPosition(pos);
	}

	/**
	 * Writes a string but doesn't moves the caret.
	 */
	inline void Insert(const TString& value)
	{
		GetPosition();
		Write(value);
		SetPosition(Cursor);
	}



	typedef delegate1<void,ch32> KeyPressEvent;
	event<KeyPressEvent> KeyPress;

	/**
	 * Event based get key.
	 */
	//virtual void BeginGetKey( delegate1<void,ch32>* callback ) = 0;

	
	/**
	 * Blocking get key.
	 */
	//virtual ch32 GetKey() = 0;
	
	/**
	 * Blocking get string.
	 */
	//virtual TString GetString() = 0;

	/**
	 * Event based get string.
	 */
	//virtual void BeginGetString( delegate1<void,const TString&>* callback ) = 0;
	
};



#endif