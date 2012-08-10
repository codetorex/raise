#ifndef TMOUSEMAPPED_H
#define TMOUSEMAPPED_H

#include "tinputmap.h"


class TMouseMapped: public IMouseObserver
{
public:
	TButtonSensor Buttons[5];
	TPositionSensor X;
	TPositionSensor Y;

	void BindButton(int buttonID, TTriggerAction* Action)
	{
		Buttons[buttonID].BindAction(Action);
	}

	void UnbindKey(int buttonID, TTriggerAction* Action)
	{
		Buttons[buttonID].UnbindAction();
	}

	inline void MouseMove(int x,int y)
	{
		X = x;
		Y = y;
	}

	inline void MouseDown(int x,int y, int button)
	{
		X = x;
		Y = y;
		Buttons[button].Down();
	}

	inline void MouseUp(int x,int y, int button)
	{
		X = x;
		Y = y;
		Buttons[button].Up();
	}

	inline void MouseWheel(int x,int y,int delta)
	{
		X = x;
		Y = y;
		// wtf we going to do with wheel?
	}
};

#endif