#ifndef TAPPLICATION_H
#define TAPPLICATION_H

#include "tstring.h"

class TCultureInfo;

class TVersionPhase
{
public:
	int		ID;
	TString Name;
	TString ShortName;

	static TVersionPhase PreAlpha;
	static TVersionPhase Alpha;
	static TVersionPhase Beta;
	static TVersionPhase ReleaseCandidate;
	static TVersionPhase Release;

	TVersionPhase(int _id, const TString& _name, const TString& _shortname)
	{
		ID = _id;
		Name = _name;
		ShortName = _shortname;
	}
};

class TVersion
{
private:
	void SetBuildText();
	void SetVersionText();

public:
	enum TVersionScheme
	{
		V_ALL_USER, // 1.0.0 all parameters given by user
		V_MAJ_MIN_USER, // 1.0.0 , first 1 and 0 is user given last number is build number
		V_BUILDGENERATED, // 0.1.42 means build 142
	};

	int Major;
	int Minor;
	int SubMinor;
	int Build; // build number

	const TVersionPhase* Phase;

	TVersionScheme Scheme;

	TString BuildText; // like "build 512"
	TString BuildPhaseText; // like "build 512 alpha"
	TString VersionText; // like 0.1.512
	TString VersionPhaseText; // like 0.1.512 alpha

	void Initialize(int _build, const TVersionPhase* _phase);

	void Initialize(int _maj, int _min, int _build, const TVersionPhase* _phase);

	void Initialize(int _maj, int _min, int _sub, int _build, const TVersionPhase* _phase);

	/**
	 * Void constructor.
	 */
	TVersion() {}

	/**
	 * Creates version information out of build number.
	 * Revision will be maximum 99, when its 100 then 0.1.0 happens
	 * so 1000 th build will be 1.0.0 and 1001 th will be 1.0.1 and 1020th is 1.0.20
	 * and 1120 th will be 1.1.20
	 */
	TVersion(int _build, const TVersionPhase* _phase);

	/**
	 * Major and minor will be user defined. But sub minor will be build number.
	 */
	TVersion(int _maj, int _min, int _build, const TVersionPhase* _phase);

	/**
	 * Major minor and sub minor are user defined and build is different number.
	 */
	TVersion(int _maj, int _min, int _sub, int _build, const TVersionPhase* _phase);
};

class TModule
{
public:
	TString Name;
	TString Company;
	TVersion Version;

	TString IdentifyText;
	TString IdentifyBuildText;

	TModule() {};

	TModule(const TString& _Name, TVersion& _Version, const TString& _Company);

	TModule(const TString& _Name, TVersion& _Version);

	void SetIdentifyText();
};

class TApplication: public TModule
{
public:
	TString ExecutablePath;
	TString StartupPath;

	static TCultureInfo& CurrenctCulture();

	TArray< TModule* > Modules;

	void Exit();

	/**
	 * Easier setup for initializing application data.
	 */
	void Begin( TString _Name, TVersion& _Version, TString _Company );
	
	/**
	 * Uses default company name while initializing application data.
	 */
	void Begin( TString _Name, TVersion& _Version ); 
};

extern TApplication Application;



/*
class TModule
{
public:

};
*/

#endif