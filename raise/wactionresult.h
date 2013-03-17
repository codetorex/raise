#ifndef WACTIONRESULT_H
#define WACTIONRESULT_H

#include "whttpcontext.h"

/**
 * Action result interface
 */
class WActionResult
{
public:
	virtual void PerformResult(WHttpContext* ctx) = 0;
};

#endif