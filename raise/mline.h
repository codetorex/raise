#ifndef MLINE_H
#define MLINE_H

#include "mvector2.h"

template <class T>
class MLine2
{
public:
	MVector2<T> StartPoint;
	MVector2<T> EndPoint;

	inline T Length()
	{
		return StartPoint.Distance(EndPoint);
	}
};

typedef MLine2<float> Line;

typedef MLine2<int> IntLine;

#endif