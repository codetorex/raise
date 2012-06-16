#ifndef TPLATFORM_H
#define TPLATFORM_H


// TODO: derive this from current platfomr, define TPlatform, and implement platform related stuff in there

class TTypeManager;


enum EDialogResult
{
	TDR_None,
	TDR_OK,
	TDR_Cancel,
	TDR_Abort,
	TDR_Retry,
	TDR_Ignore,
	TDR_Yes,
	TDR_No,
};

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

	virtual void RaiseToSystemString(const TString& raiseStr, byte* output, ui32 outputSize) = 0;

	virtual TString SystemToRaiseString(byte* input, ui32 inputSize) = 0;

	virtual TString GetErrorDescription(ui32 errorID);

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
class TPlatform // : public TPlatformCurrent
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