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
	TString Fragment;
	int Type;
};

class WFalconRenderer:public WViewRenderer
{
protected:
	void AddFragment(int type, const TString& content);
	void ParseCode(TStringReader& sr);
	void ParseParens(TStringReader& sr);
	bool IsKeyword(const TString& str);

public:
	TString Source;
	TString SourcePath;

	TArray< WFalconFragment* > Fragments;

	void LoadSource(const TString& srcPath);

	inline void SetSource(const TString& src)
	{
		Source = src;
	}

	void ParseSource();

	

	void Render(WController* c, WModel* model);
};

#endif