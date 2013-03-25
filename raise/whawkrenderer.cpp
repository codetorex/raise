#include "stdafx.h"
#include "whawkrenderer.h"
#include "ttextreader.h"

void WHawkRenderer::LoadSource( TTextReader& source )
{
	TStringBuilder textPart;

	ch32 curChar;
	while (!source.EndOfStream)
	{
		curChar = source.Read();
		if (curChar == '<')
		{

		}
	}
}

void WHawkRenderer::Render( WController* c, WModel* model )
{

}

