#ifndef WFALCONRENDERER_H
#define WFALCONRENDERER_H

#include "wviewrenderer.h"

class WModel;
class WController;
class TStringReader;

enum FalconFragmentTypes
{
	FFT_TEXT,
	FFT_CODE,
};

class WFalconFragment
{
public:
	String Fragment;
	int Type;
};

class WFalconRenderer:public WViewRenderer
{
protected:
	void AddFragment(int type, const String& content);
	void ParseCode(TStringReader& sr);
	void ParseParens(TStringReader& sr);
	bool IsKeyword(const String& str);

public:
	String Source;
	String SourcePath;

	Array< WFalconFragment* > Fragments;

	void LoadSource(const String& srcPath);

	inline void SetSource(const String& src)
	{
		Source = src;
	}

	void ParseSource();

	

	void Render(WController* c, WModel* model);
};

#endif