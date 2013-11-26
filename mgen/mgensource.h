#ifndef MGENSOURCE_H
#define MGENSOURCE_H

#include "mgenelements.h"

class MGenSourceLine
{
public:
	int				LineNumber;
	String			Line;
};

class MGenDatabase;

class MGenToken
{
public:

	MGenToken*			Parent;
	String				Token;
	Array<MGenToken*>	Childs;

	inline void AddChild(MGenToken* child)
	{
		Childs.Add(child);
		child->Parent = this;
	}

	inline MGenToken* AddChild(const String& value)
	{
		MGenToken* nchild = new MGenToken();
		nchild->Token = value;
		AddChild(nchild);
		return nchild;
	}
};


class MGenSource
{
public:
	MGenDatabase* Parent;

	String SourcePath;

	Array<MGenSourceLine*> Lines;

	/// Holds source pointers that this source done #include
	Array<MGenSource*> Includes;

	Array<MGenClass*>	Classes;

	Array<MGenEnum*>	Enums;

	MGenToken			Root;
};


#endif