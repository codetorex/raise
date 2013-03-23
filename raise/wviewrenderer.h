#ifndef WVIEWRENDERER_H
#define WVIEWRENDERER_H

#include "whttpcontext.h"

class WController;
class WModel;

class WViewRenderer
{
public:
	virtual void Render(WController* c, WModel* model) = 0;
};

#endif