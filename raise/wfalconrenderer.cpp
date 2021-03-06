#include "stdafx.h"
#include "wfalconrenderer.h"

#include "wcontroller.h"

#include "tstreamreader.h"
#include "wmvcapplication.h"
#include "tstringreader.h"

void WFalconRenderer::Render( WController* c, WModel* model )
{
	String fullPath = c->MVCApplication->ApplicationPath + c->CurrentAction->ViewPath;

	TStreamReader* sr = new TStreamReader(fullPath);
	String content = sr->ReadToEnd();
	sr->Close();

	c->Response->OutputStream->Write(content.Data,1,content.Length);
}

void WFalconRenderer::LoadSource( const String& srcPath )
{
	SourcePath = srcPath;
	TStreamReader sr(SourcePath);
	SetSource(sr.ReadToEnd());
	sr.Close();
}

void WFalconRenderer::ParseSource()
{
	TStringReader sr(Source);

	while (true)
	{
		int interrupt;
		String textPart = sr.ReadUntil("@",String::Empty,interrupt);
		if (interrupt == '@')
		{
			ch32 nextChar = sr.Peek();
			if (nextChar == '@')
			{
				sr.Read();
				textPart.AppendASCII('@');
			}
			if (textPart.Length > 0)
			{
				AddFragment(FFT_TEXT, textPart);
			}

			ParseCode(sr);
		}

		if (interrupt == -1)
		{
			if (textPart.Length > 0)
			{
				AddFragment(FFT_TEXT, textPart);
			}
			break;
		}
	}

}

void WFalconRenderer::ParseCode( TStringReader& sr )
{
	int interrupt;
	String codePart = sr.ReadUntil("( \"><{", String::Empty, interrupt);

	if (interrupt == '(' || interrupt == ' ')
	{
		if (IsKeyword(codePart))
		{
			// take whole line
			String rest = sr.ReadLine();
		}
	}

}

bool WFalconRenderer::IsKeyword( const String& str )
{
	if (str == "model")
		return true;

	if (str == "foreach")
		return true;

	return false;
}



void WFalconRenderer::AddFragment( int type, const String& content )
{
	WFalconFragment* frg = new WFalconFragment();
	frg->Fragment = content;
	frg->Type = type;
	Fragments.Add(frg);
}

