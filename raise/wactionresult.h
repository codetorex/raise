#ifndef WACTIONRESULT_H
#define WACTIONRESULT_H

#include "waction.h"

class WController;
class WViewRenderer;
class WModel;

/**
 * Action result interface
 */
class WActionResult
{
public:
	virtual void PerformResult(WController* c) = 0;
};

class WRedirectResult: public WActionResult
{
public:
	TString RedirectPath;

	WRedirectResult(const TString& newPath)
	{
		RedirectPath = newPath;
	}

	void PerformResult(WController* c);
};

class WViewResult: public WActionResult
{
public:
	WViewRenderer* Renderer;
	WModel* Model;

	WViewResult(WViewRenderer* renderer, WModel* model)
	{
		this->Renderer = renderer;
		this->Model = model;
	}

	void PerformResult(WController* c);
};

class WJsonResult: public WActionResult
{
	void PerformResult(WController* c);
};

#endif