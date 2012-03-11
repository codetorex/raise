#ifndef TINPUTMAP_H
#define TINPUTMAP_H

#include "tevent.h"
#include "tmouse.h"
#include "tkeyboard.h"

// NOTE: these Actions will be result of sensor input. CSensor -> Take Data -> CAction
// ***************************************************************
// Those are actions for output....
// ***************************************************************

// TODO: there should be actions defined and Inputs can be set for that actions.

class TInputAction
{
public:
	TString actionName;
};

class TTriggerAction: public TInputAction
{
public:
	virtual void Trigger(bool value) = 0; // button press will use this
};

class TAxisAction: public TInputAction
{
	virtual void Axis(float value) = 0; // axis of joystick will use this
};

class TMoveAction: public TInputAction
{
	virtual void Move(int value) = 0; // mouse will use this?
};

/**
* Input is digital, like true or false signals. But output will be analog between 0.0f and 1.0f.
*/
class TDiglogAction: public TTriggerAction
{
public:

};


/**
* This one can be used to implement flag based action map.
* Which is used in quake?
*/
class TFlagAction: public TTriggerAction
{
public:
	ui32* flagAddr;
	ui32 bit;

	TFlagAction(ui32* _flagAddr, ui32 _bit)
	{
		flagAddr = _flagAddr;
		bit = _bit;
	}

	inline void Trigger(bool value)
	{
		if (value)
		{
			*flagAddr |= bit;
		}
		else
		{
			*flagAddr &= ~bit;
		}
	}
};

class TBoolChangerAction: public TTriggerAction
{
public:
	bool* valueToChange;

	TBoolChangerAction(bool* dataPtr)
	{
		valueToChange = dataPtr;
	}

	inline void Trigger(bool value)
	{
		*valueToChange = value;
	}
};

class TFloatChangerAction: public TTriggerAction
{
public:
	float* valueToChange;

	TFloatChangerAction(float* dataPtr)
	{
		valueToChange = dataPtr;
	}

	inline void Axis(float value)
	{
		*valueToChange = value;
	}
};

/*class RDLL CValueChangerAction: public CAction
{
public:
	void* valueToChange;

	CValueChangerAction(void* dataPtr)
	{
		valueToChange = dataPtr;
	}

	inline void Trigger(bool value)
	{
		*(bool*)valueToChange = value;
	}

	inline void Axis(float value)
	{
		*(float*)valueToChange = value;
	}

	inline void Move(int value)
	{
		*(int*)valueToChange = value;
	}
};*/

class TEventAction: public TTriggerAction
{
public:

	typedef delegate1<void,bool> TriggerEvent;

	event<TriggerEvent> OnTrigger;

	TEventAction()
	{

	}

	inline void Trigger(bool value)
	{
		OnTrigger.call(value);
	}
};

// ***************************************************************
// Those are sensors for input....
// ***************************************************************

/**
* CSensor class for detecting device input.
* Class T is CAction result of sensor.
*/

template <class T>
class RDLL TInputSensor
{
public:
	T* Action;

	TInputSensor()
	{
		Action = 0;
	}

	inline void BindAction(T* action)
	{
		Action = action;
	}

	inline void UnbindAction()
	{
		Action = 0;
	}
};

/**
* Digital boolean sensor.
*/
class RDLL TButtonSensor: public TInputSensor<TTriggerAction>
{
public:
	bool state; // true for down, false for up

	TButtonSensor()
	{
		state = false;
	}

	inline void Down()
	{
		if ( Action == 0 ) return;
		Action->Trigger(true);
	}

	inline void Up()
	{
		if ( Action == 0 ) return;
		Action->Trigger(false);
	}
};

/**
* Floating point based sensor.
*/
class RDLL TAxisSensor: public TInputSensor<TAxisAction>
{

};

/**
* Integer based sensor.
*/
class RDLL TPositionSensor: public TInputSensor<TMoveAction>
{
public:
	int Absolute;
	int Diff;

	TPositionSensor()
	{
		Absolute = 0;
		Diff = 0;
	}

	TPositionSensor(const int val)
	{
		Absolute = val;
		Diff = 0;
	}

	inline void Set(int newPos)
	{
		Diff = newPos - Absolute;
		Absolute = newPos;
	}

	inline TPositionSensor& operator = (int newPos)
	{
		Diff = newPos - Absolute;
		Absolute = newPos;
		return *this;
	}

	inline operator int (void)
	{
		return Absolute;
	}
};

class TMappedKeyboard: public IKeyboardObserver
{
public:
	TButtonSensor Keys[256];

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
		Keys[keyID].Down();
	}

	void KeyUp(int keyID)
	{
		Keys[keyID].Up();
	}

	void KeyUnicode(ch16 keyChar)
	{
		// HMM... Not any known uses? Maybe used for mapping stuff in non english keyboards?
	}
};

class TMappedMouse: public IMouseObserver
{
public:
	TButtonSensor Buttons[5];
	TPositionSensor X;
	TPositionSensor Y;

	void MouseMove(int x,int y)
	{
		X = x;
		Y = y;
	}

	void MouseDown(int x,int y, int button)
	{
		X = x;
		Y = y;
		Buttons[button].Down();
	}

	void MouseUp(int x,int y, int button)
	{
		X = x;
		Y = y;
		Buttons[button].Up();
	}

	void MouseWheel(int x,int y,int delta)
	{
		X = x;
		Y = y;
		// wtf we going to do with wheel?
	}
};

#endif