#ifndef WFALCONRENDERER_H
#define WFALCONRENDERER_H

#include "wviewrenderer.h"

class WModel;
class WController;

class WFalconRenderer:public WViewRenderer
{
public:
	void Render(WController* c, WModel* model);
};

#endif