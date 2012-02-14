#ifndef MGENSOURCE_H
#define MGENSOURCE_H

#include "mgenelements.h"

class MGenSourceLine
{
public:
	int				LineNumber;
	TString			Line;
};

class MGenDatabase;

class MGenToken
{
public:

	MGenToken*			Parent;
	TString				Token;
	TArray<MGenToken*>	Childs;

	inline void AddChild(MGenToken* child)
	{
		Childs.Add(child);
		child->Parent = this;
	}

	inline MGenToken* AddChild(const TString& value)
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

	TString SourcePath;

	TArray<MGenSourceLine*> Lines;

	/// Holds source pointers that this source done #include
	TArray<MGenSource*> Includes;

	TArray<MGenClass*>	Classes;

	TArray<MGenEnum*>	Enums;

	MGenToken			Root;
};


#endif