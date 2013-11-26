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

	String Fragment;
	int Type;
};

class WHawkRenderer:public WViewRenderer
{
private:
	void AddFragment(int type, const String& content);

public:
	String CodeStart;
	String CodeEnd;

	WHawkRenderer()
	{
		CodeStart = "<$";
		CodeEnd = "$>";
	}

	Array< WHawkFragment* > Fragments;

	void ParseSource(TTextReader& source);
	void CreateCode();
	void CompileCode();

	void Render(WController* c, WModel* model);
};

#endif