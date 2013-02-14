#ifndef TPLATFORM_H
#define TPLATFORM_H


#ifdef WIN32
#include "tplatformwin32.h"
#else
#include "tplatformunix.h"
#endif // WIN32


// TODO: derive this from current platfomr, define TPlatform, and implement platform related stuff in there

class TTypeManager;




/*enum ECursorSystem
{
	CS_BUSY = IDC_APPSTARTING,
	CS_ARROW = IDC_ARROW,
	CS_CROSSHAIR = IDC_CROSS,

};*/


/**
 * This class is base for platform depended functions
 */
/*class TPlatformInterface
{
public:
	virtual void InitializePlatform() = 0;

	virtual EDialogResult ShowMessageBox(const TString& text, const TString& caption, int button, int icon) = 0;

	virtual EDialogResult BrowseFolder(const TString& startPath, TString& OUT selectedPath);


};

class TPlatformWindows: public TPlatformInterface
{
public:

};

typedef TPlatformWindows TPlatformCurrent;

/**
 * Main system façade to do everything?
 */
class TPlatform: public TPlatformCurrent
{
public:
	/**
	 * Initialize everything about system here?
	 */
	void Initialize();

	TTypeManager* Type;
};

extern TPlatform Platform;

#endif