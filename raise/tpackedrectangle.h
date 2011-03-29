#ifndef TPACKEDTRIANGLE_H
#define TPACKEDTRIANGLE_H

#include "tregion.h"

class TPackedRectangle;
typedef TPackedRectangle TRectangleNode;

class TPackedRectangle: public TRegion
{
public:
	bool			LeftEmpty;
	bool			RightEmpty;

	TRectangleNode*	LeftBranch;
	TRectangleNode*	RightBranch;

	TPackedRectangle();
	TPackedRectangle(int _width, int _height);
	~TPackedRectangle();

	TRectangleNode* Insert( int rwidth,int rheight );

	inline TRectangleNode* Insert( TRange* range )
	{
		return Insert(range->Width,range->Height);
	}
};


#endif