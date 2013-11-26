#include "stdafx.h"
#include "whawkrenderer.h"
#include "ttextreader.h"

void WHawkRenderer::ParseSource( TTextReader& source )
{
	TStringBuilder textPart;

	ch32 curChar;
	while (!source.EndOfStream)
	{
		String textPart = source.ReadUntilString(CodeStart);
		if (textPart.Length > 0)
		{
			AddFragment( WHawkFragment::HFTYP_TEXT, textPart);
		}

		if (!source.EndOfStream)
		{
			String codePart = source.ReadUntilString(CodeEnd);
			AddFragment( WHawkFragment::HFTYP_CODE, codePart);
		}
	}
}

void WHawkRenderer::AddFragment( int type, const String& content )
{
	WHawkFragment* frg = new WHawkFragment();
	frg->Type = type;
	frg->Fragment = content;
	Fragments.Add(frg);
}

void WHawkRenderer::Render( WController* c, WModel* model )
{

}

void WHawkRenderer::CreateCode()
{
	TStringBuilder code;
	code.AppendLine("#include \"wmvc.h\"");
	code.AppendLine();
}

