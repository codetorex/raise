#ifndef TJOYSTICK_H
#define TJOYSTICK_H

#include "tinputdevice.h"

class IJoystickObserver
{
public:
	virtual void JoystickKey(int keyID) = 0;
	virtual void JoystickAxis(int axis, float value) = 0;
};


class TJoystick: public TInputDevice
{
public:
	// Not Implemented Yet.
};

#endif