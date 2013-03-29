#include "stdafx.h"
#include "whawkrenderer.h"
#include "ttextreader.h"

void WHawkRenderer::ParseSource( TTextReader& source )
{
	TStringBuilder textPart;

	ch32 curChar;
	while (!source.EndOfStream)
	{
		TString textPart = source.ReadUntilString(CodeStart);
		if (textPart.Length > 0)
		{
			AddFragment( WHawkFragment::HFTYP_TEXT, textPart);
		}

		if (!source.EndOfStream)
		{
			TString codePart = source.ReadUntilString(CodeEnd);
			AddFragment( WHawkFragment::HFTYP_CODE, codePart);
		}
	}
}

void WHawkRenderer::AddFragment( int type, const TString& content )
{
	WHawkFragment* frg = new WHawkFragment();
	frg->Type = type;
	frg->Fragment = content;
	Fragments.Add(frg);
}

void WHawkRenderer::Render( WController* c, WModel* model )
{

}

