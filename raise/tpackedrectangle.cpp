#include "stdafx.h"
#include "tpackedrectangle.h"

TPackedRectangle::TPackedRectangle()
{
	LeftEmpty = true;
	RightEmpty = true;
	LeftBranch = NULL;
	RightBranch = NULL;
}

TPackedRectangle::TPackedRectangle(int _width, int _height)
{
	LeftEmpty = true;
	RightEmpty = true;
	LeftBranch = NULL;
	RightBranch = NULL;
	
	SetRectangle(0,0,_width,_height);
}

TPackedRectangle::~TPackedRectangle()
{
	if( LeftBranch != 0 )
	{
		delete LeftBranch;
	}

	if ( RightBranch != 0 )
	{
		delete RightBranch;
	}
}

TRectangleNode* TPackedRectangle::Insert( int rwidth,int rheight )
{
	TRectangleNode* newNode;
	if (LeftBranch != 0 && RightBranch != 0) // we're not a leaf
	{
		newNode = LeftBranch->Insert(rwidth,rheight);
		if (newNode != NULL) return newNode;

		return RightBranch->Insert(rwidth,rheight);
	}
	else // we are leaf
	{
		if (!LeftEmpty && !RightEmpty) // this is not empty area
		{
			return NULL;
		}	

		if (rwidth > Width || rheight > Height) // too big
		{
			return NULL;	
		}

		if (rwidth == Width && rheight == Height ) // fits exactly
		{
			LeftEmpty = false;
			RightEmpty = false;
			return this;
		}

		// we gone split it to kids
		LeftBranch = new TRectangleNode();
		RightBranch = new TRectangleNode();

		long dw,dh;
		dw = Width - rwidth;
		dh = Height - rheight;

		if ( dw > dh)
		{
			LeftBranch->SetRectangle(X,Y,rwidth,Height);
			RightBranch->SetRectangle(X+rwidth,Y,Width - rwidth,Height);
		}
		else
		{
			LeftBranch->SetRectangle(X,Y,Width,rheight);	
			RightBranch->SetRectangle(X,Y+rheight,Width,Height - rheight);
		}

		return LeftBranch->Insert(rwidth,rheight);
	}
}

