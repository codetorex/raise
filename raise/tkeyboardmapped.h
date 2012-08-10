#ifndef TKEYBOARDMAPPED_H
#define TKEYBOARDMAPPED_H

#include "tinputmap.h"

/**
 * Simple keyboard event handling class
 */
class TKeyboardMapped: public IKeyboardObserver
{
public:
	TButtonSensor Keys[256];

	TKeyboardMapped()
	{
		Parent = 0;
	}

	void BindKey(int KeyID, TTriggerAction* Action)
	{
		Keys[KeyID].BindAction(Action);
	}

	void UnbindKey(int KeyID)
	{
		Keys[KeyID].UnbindAction();
	}

	void KeyDown(int keyID)
	{
		return Keys[keyID].Down();
	}

	void KeyUp(int keyID)
	{
		return Keys[keyID].Up();
	}

	void KeyUnicode(ch16 keyChar)
	{
		// HMM... Not any known uses? Maybe used for mapping stuff in non english keyboards?
	}
};

#endif