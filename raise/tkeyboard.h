#ifndef TKEYBOARD_H
#define TKEYBOARD_H

#include "tinputenum.h"
#include "tinputdevice.h"

class IKeyboardObserver
{
public:
	virtual void KeyDown(int keyID) = 0;
	virtual void KeyUp(int keyID) = 0;
	virtual void KeyUnicode(ch16 keyChar) = 0;
};

/**
* Mapped keyboard input.
*/
class TKeyboard: public TInputDevice
{
public:
	Array< IKeyboardObserver* > Observers;

	TKeyboard()
	{
		inputName = "Keyboard";
	}

	inline void Attach( IKeyboardObserver* Observer )
	{
		Observers.Add(Observer);
	}

	inline void Detach( IKeyboardObserver* Observer )
	{
		Observers.Remove(Observer);
	}

	inline void KeyDown(int keyID)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Items[i]->KeyDown(keyID);
		}
	}

	inline void KeyUp(int keyID)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Items[i]->KeyUp(keyID);
		}
	}

	inline void KeyUnicode(ch16 keyChar)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Items[i]->KeyUnicode(keyChar);
		}
	}
};



#endif