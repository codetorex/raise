#ifndef TINPUTMAP_H
#define TINPUTMAP_H

#include "tevent.h"
#include "tmouse.h"
#include "tkeyboard.h"
#include "texceptionlow.h"

// NOTE: these Actions will be result of sensor input. CSensor -> Take Data -> CAction
// ***************************************************************
// Those are actions for output....
// ***************************************************************

// TODO: there should be actions defined and Inputs can be set for that actions.


class TButtonSensor;
class TPositionSensor;

class TInputModeBase;


/// THE FOLLOWING CLASSES IS NEW AND IMPROVED SYSTEMS
enum TInputActionTypes
{
	IA_BUTTON,
	IA_POSITION,
	IA_AXIS,
};

class TInputAction
{
public:
	TString Name; // name of input, like FlyCamX, FlyCamY, FlyCamAscend ?
	int ActionType;
};

class TTriggerAction: public TInputAction
{
public:
	TTriggerAction()
	{
		ActionType = IA_BUTTON; 
	}

	bool Value;

	virtual void Trigger(bool value)
	{
		this->Value = value;
	}
};

class TTriggerActionEvent: public TTriggerAction
{
public:
	typedef delegate0<void> NoParamEvent;
	typedef delegate1<void,bool> BoolEvent;

	event<NoParamEvent> KeyDown;
	event<NoParamEvent> KeyUp;
	event<BoolEvent>	KeyPress;

	void Trigger(bool value)
	{
		if (value)
		{
			KeyDown.call();
		}
		else
		{
			KeyUp.call();
		}

		KeyPress.call(value);
	}
};

class TAxisAction: public TInputAction
{
public:
	float Value;
	float Difference;
	
	void Axis(float value)
	{
	
	}// axis of joystick will use this
};

class TMoveAction: public TInputAction
{
public:
	TMoveAction()
	{
		ActionType = IA_POSITION;
	}

	int Position;
	int Difference;

	virtual void Move(int value)
	{
		Difference = value - Difference;
		Position = value;
	}
};


class TInputModeBase
{
public:
	TString Name;
	TArray< TInputAction* > Actions; // actions that uses this mode

	TTriggerAction* CreateTriggerAction(const TString& name)
	{
		TTriggerAction* newAction = new TTriggerAction();
		newAction->Name = name;
		Actions.Add(newAction);
		return newAction;
	}

	TMoveAction* CreateMoveAction(const TString& name)
	{
		TMoveAction* newAction = new TMoveAction();
		newAction->Name = name;
		Actions.Add(newAction);
		return newAction;
	}

	TInputAction* GetAction(const TString& name, int actionType)
	{
		TArrayEnumerator<TInputAction*> e(Actions);
		while(e.MoveNext())
		{
			if (e.Current->Name == name)
			{
				if (e.Current->ActionType == actionType)
				{
					return e.Current;
				}
			}
		}

		return 0;
	}

	TTriggerAction* GetTriggerAction(const TString& name)
	{
		return (TTriggerAction*)GetAction(name,IA_BUTTON);
	}

	TMoveAction* GetMoveAction(const TString& name)
	{
		return (TMoveAction*)GetAction(name,IA_POSITION);
	}
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

/*class TFloatChangerAction: public TTriggerAction
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
};*/

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
	bool State; // true for down, false for up

	TButtonSensor()
	{
		State = false;
	}

	inline bool Down()
	{
		if ( Action == 0 ) return false;
		Action->Trigger(true);
		return true;
	}

	inline bool Up()
	{
		if ( Action == 0 ) return false;
		Action->Trigger(false);
		return true;
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
		Reset();
	}

	TPositionSensor(const int val)
	{
		Absolute = val;
		Diff = 0;
	}

	inline void Reset()
	{
		Diff = 0;
		Absolute = 0;
	}

	inline void Set(int newPos)
	{
		
		Diff = newPos - Absolute;
		Absolute = newPos;
		if (Action)
		{
			Action->Move(newPos);
		}
	}

	inline TPositionSensor& operator = (int newPos)
	{
		Set(newPos);
		return *this;
	}

	inline operator int (void)
	{
		return Absolute;
	}
};

#endif