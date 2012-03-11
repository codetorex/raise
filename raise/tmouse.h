#ifndef TMOUSE_H
#define TMOUSE_H

#include "tinputenum.h"
#include "tinputdevice.h"

class IMouseObserver
{
public:
	virtual void MouseMove(int x,int y) = 0;
	virtual void MouseDown(int x,int y, int button) = 0;
	virtual void MouseUp(int x,int y, int button) = 0;
	virtual void MouseWheel(int x,int y,int delta) = 0;
};

class TMouse: public TInputDevice
{
public:
	TArray< IMouseObserver* > Observers;

	TMouse()
	{
		inputName = "Mouse";
	}

	inline void Attach( IMouseObserver* Observer )
	{
		Observers.Add(Observer);
	}

	inline void Detach( IMouseObserver* Observer )
	{
		Observers.Remove(Observer);
	}

	inline void MouseMove(int _x,int _y)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseMove(_x,_y);
		}
	}

	inline void MouseDown(int _x,int _y,int _button)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseDown(_x,_y,_button);
		}
	}

	inline void MouseUp(int _x,int _y,int _button)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseUp(_x,_y,_button);
		}
	}

	inline void MouseWheel(int _x, int _y, int _delta)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseWheel(_x,_y,_delta);
		}
	}
};

#endif