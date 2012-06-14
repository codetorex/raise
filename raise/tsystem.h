#ifndef TSYSTEM_H
#define TSYSTEM_H


// TODO: derive this from current platfomr, define TPlatform, and implement platform related stuff in there

class TTypeManager;


/**
 * Main system façade to do everything?
 */
class TSystem
{
public:
	/**
	 * Initialize everything about system here?
	 */
	void Initialize();

	TTypeManager* Type;
};

extern TSystem System;

#endif