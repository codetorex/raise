#ifndef WACTION_H
#define WACTION_H

#include "wmodel.h"
#include "tevent.h"
#include "tstring.h"

class WActionResult;


class WAction
{
public:
	typedef delegate1< WActionResult* , WModel* > ActionDelegate;

	TString Name;

	TString ViewPath;

	ActionDelegate* ModelAction;

	WAction(const TString& name, ActionDelegate* modelaction )
	{
		this->Name = name;
		ModelAction = modelaction;
	}
};

#endif