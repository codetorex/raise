#include "stdafx.h"
#include "tapplication.h"
#include "tpath.h"
#include "raiseversion.h"
#include "tstringformat.h"

TVersionPhase TVersionPhase::PreAlpha			(0, "Pre-alpha",		"pre");
TVersionPhase TVersionPhase::Alpha				(1, "Alpha",			"alpha");
TVersionPhase TVersionPhase::Beta				(2, "Beta",				"beta");
TVersionPhase TVersionPhase::ReleaseCandidate	(3, "Release Candidate","rc");
TVersionPhase TVersionPhase::Release			(4, "Release",			"r");

void TVersion::SetBuildText()
{
	BuildText			= TString::Format("build %", sfi(SubMinor));
	BuildPhaseText		= TString::Format("build % %", sfi(SubMinor), sfs(Phase->ShortName));
}

void TVersion::SetVersionText()
{
	VersionText			= TString::Format("%.%.%",sfi(Major),sfi(Minor),sfi(SubMinor));
	VersionPhaseText	= TString::Format("% %", sfs(VersionText), sfs(Phase->ShortName));
}

void TVersion::Initialize( int _build, const TVersionPhase* _phase )
{
	Build = _build;
	SubMinor = Build % 100;
	Phase = _phase;
	int duzhesap = (Build - SubMinor) / 100;
	Minor = duzhesap % 10;
	Major = (duzhesap - Minor) / 10;

	SetBuildText();
	SetVersionText();

	Scheme = V_BUILDGENERATED;
}

void TVersion::Initialize( int _maj, int _min, int _build, const TVersionPhase* _phase )
{
	Initialize(_maj,_min,_build,_build,_phase);
	Scheme = V_MAJ_MIN_USER;
}

void TVersion::Initialize( int _maj, int _min, int _sub, int _build, const TVersionPhase* _phase )
{
	Major = _maj;
	Minor = _min;
	SubMinor = _sub;
	Build = _build;
	Phase = _phase;

	SetBuildText();
	SetVersionText();

	Scheme = V_ALL_USER;
}


TVersion::TVersion( int _build, const TVersionPhase* _phase )
{
	Initialize(_build,_phase);
}

TVersion::TVersion( int _maj, int _min, int _build, const TVersionPhase* _phase )
{
	Initialize(_maj,_min,_build,_phase);
}

TVersion::TVersion( int _maj, int _min, int _sub, int _build, const TVersionPhase* _phase )
{
	Initialize(_maj,_min,_sub,_build,_phase);
}

TModule::TModule( const TString& _Name, TVersion& _Version, const TString& _Company )
{
	Name = _Name;
	Version = _Version;
	Company = _Company;
	SetIdentifyText();
}

TModule::TModule( const TString& _Name, TVersion& _Version )
{
	Name = _Name;
	Version = _Version;
	Company = RAISE_DEFAULT_COMPANYNAME;
	SetIdentifyText();
}

void TModule::SetIdentifyText()
{
	IdentifyText = TString::Format("% - % - %", sfs(Name), sfs(Version.VersionPhaseText), sfs(Company));
	IdentifyBuildText = TString::Format("% - % - %", sfs(Name), sfs(Version.BuildPhaseText), sfs(Company));
}

TModule RaiseModule("Raise",TVersion(RAISE_BUILD,RAISE_PHASE));

TApplication Application;

void TApplication::Begin( TString _Name, TVersion& _Version, TString _Company )
{
	StartupPath = TPathDriver::CurrentDirectory();
	ExecutablePath = "Not Implemented";

	Name = _Name;
	Version = _Version;
	Company = _Company;

	Application.Modules.Add(&RaiseModule);
	SetIdentifyText();
}

void TApplication::Begin( TString _Name, TVersion& _Version )
{
	Begin( _Name,_Version, RAISE_DEFAULT_COMPANYNAME );
}

