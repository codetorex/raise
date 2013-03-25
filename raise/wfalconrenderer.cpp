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
	/*int i = 0;
	while (i != Source.Length)
	{
		//TString textPart = Source.SubstringUntil(i,'@');

	}*/
	

}

void WFalconRenderer::AddFragment( int type, const TString& content )
{
	WFalconFragment* frg = new WFalconFragment();
	frg->Fragment = content;
	frg->Type = type;
	Fragments.Add(frg);
}
