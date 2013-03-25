#ifndef WHAWKRENDERER_H
#define WHAWKRENDERER_H


#include "wviewrenderer.h"

class WModel;
class WController;
class TTextReader;



class WHawkFragment
{
public:
	enum HawkFragmentTypes
	{
		HFTYP_TEXT,
		HFTYP_CODE,
	};

	TString Fragment;
	int Type;
};

class WHawkRenderer:public WViewRenderer
{
private:
	void AddFragment(int type, const TString& content);

public:
	TString CodeStart;
	TString CodeEnd;

	WHawkRenderer()
	{
		CodeStart = "<$";
		CodeEnd = "$>";
	}

	TArray< WHawkFragment* > Fragments;

	void LoadSource(TTextReader& source);

	void Render(WController* c, WModel* model);
};

#endif