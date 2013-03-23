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

