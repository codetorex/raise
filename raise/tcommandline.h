#ifndef TCOMMANDLINE_H
#define TCOMMANDLINE_H

#include "tstring.h"
#include "tarray.h"
#include "tencoding.h"
#include "tutf16encoding.h"

// Command Line parameters will be handled with Chain of Responsiblity pattern

class TCommandLine;

class TCommandLineHandler
{
public:
	enum HandleResult
	{
		HR_Handled,
		HR_Continue,
	};

	/*static TArray<TCommandLineHandler*> AllHandlers;

	TCommandLineHandler()
	{
		AllHandlers.Add(this);
	}*/

	/**
	 * Tries to handle parameter returns the result.
	 */
	virtual HandleResult HandleParameter(TCommandLine* cmd) = 0;
};

class TCommandLineHandlerSimple: public TCommandLineHandler
{
public:
	TString Parameter;
	TString ShortParameter;

	virtual void ProcessParameter(TCommandLine* cmd) = 0;

	HandleResult HandleParameter(TCommandLine* cmd);
};

/*mgen network-packet __FILE__.cpp*/
/*mgen object-storage*/


/*mgen insert TPacketLogin*/
/*mgen process tpacket.h*/
/*mgen include tpacket.h*/


/**
 * Manages command line arguments.
 * mgen --input "include/test.h" --database "project.db" --mem=512k
 */
class TCommandLine
{
public:
	void* OptionObject;

	TArray< TCommandLineHandler* > Handlers;

	TArray< TString* > Parameters;
	
	/// Current parameter index that being processing
	ui32 ParameterIndex;

	inline TString& GetCurrentParameter()
	{
		return *Parameters.Item[ParameterIndex];
	}

	inline TString& GetNextParameter()
	{
		MoveNextParameter();
		return GetCurrentParameter();
	}

	inline void MoveNextParameter()
	{
		if (ParameterIndex >= Parameters.Count)
		{
			throw Exception("No more parameters in list");
		}
		ParameterIndex++;
	}

	void Initialize(int parameterCount, char* params[]);

	void Initialize(int parameterCount, ch16* params[])
	{
		for (int i=0;i<parameterCount;i++)
		{
			TString* nstring = new TString();
			*nstring = TEncoding::UTF16.GetString(params[i]);
			Parameters.Add( nstring );
		}
	}

	void Initialize(const TString& fullLine);

	TCommandLine(void* _optionObject = 0) /*, bool registerAllHandlers = true )
	{
		if (registerAllHandlers)
		{
			RegisterAllHandlers();
		}*/
	{
		OptionObject = _optionObject;
	}

	//void RegisterAllHandlers();

	void RegisterHandler(TCommandLineHandler* Handler);

	/**
	 * Parses command line parameters and dispatches handlers.
	 */
	void ParseCommandLine();
};


#endif
