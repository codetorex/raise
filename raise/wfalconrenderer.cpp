#include "stdafx.h"
#include "wfalconrenderer.h"

#include "wcontroller.h"

#include "tstreamreader.h"
#include "wmvcapplication.h"

void WFalconRenderer::Render( WController* c, WModel* model )
{
	TString fullPath = c->MVCApplication->ApplicationPath + c->CurrentAction->ViewPath;

	TStreamReader* sr = new TStreamReader(fullPath);
	string content = sr->ReadToEnd();
	sr->Close();

	c->Response->OutputStream->Write(content.Data,1,content.Length);
}

void WFalconRenderer::LoadSource( const TString& srcPath )
{
	SourcePath = srcPath;
	TStreamReader sr(SourcePath);
	SetSource(sr.ReadToEnd());
	sr.Close();
}

void WFalconRenderer::ParseSource()
{
	TStringBuilder textPart;
	TStringBuilder codePart;

	TCharacterEnumerator ce(Source);
	while (ce.MoveNext())
	{
		if (ce.Current == '@')
		{
			ch32 nextChar = ce.ReadChar();
			if (nextChar == '@')
			{
				textPart.AppendChar('@');
				continue;
			}

			if (textPart.Length > 0)
			{
				AddFragment(FFT_TEXT, textPart.ToString());
				textPart.Clear();
			}
			
			//codePart.AppendUnicode(nextChar);

			do 
			{
				if (ce.Current == '(')
				{
					ParseParens(ce,codePart);
					continue;
				}
				else if (ce.Current == ' ')
				{
					TString codePartStr = codePart.ToString();
					if (codePartStr == "model")
					{

					}
				}
				else if (ce.Current == '"' || ce.Current == '>' || ce.Current == '<' )
				{
					textPart.AppendUnicode(ce.Current);
					break;
				}

				codePart.AppendUnicode(ce.Current);
			} while (ce.MoveNext());

			AddFragment(FFT_CODE, codePart.ToString());
			codePart.Clear();
		}
		else
		{
			textPart.AppendUnicode(ce.Current);
		}
	}

	if (textPart.Length > 0)
	{
		AddFragment(FFT_TEXT, textPart.ToString());
	}
}

void WFalconRenderer::ParseParens( TCharacterEnumerator& ce, TStringBuilder& sb )
{
	sb.AppendUnicode(ce.Current);
	while (ce.MoveNext())
	{
		if (ce.Current == '(')
		{
			ParseParens(ce,sb);
		}
		if (ce.Current == ')')
		{
			sb.AppendUnicode(ce.Current);
			return;
		}

		sb.AppendUnicode(ce.Current);
	}
}

void WFalconRenderer::AddFragment( int type, const TString& content )
{
	WFalconFragment* frg = new WFalconFragment();
	frg->Fragment = content;
	frg->Type = type;
	Fragments.Add(frg);
}
