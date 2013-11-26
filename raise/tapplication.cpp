#include "stdafx.h"
#include "tapplication.h"
#include "tpath.h"
#include "tconsole.h"


TVersionPhase TVersionPhase::PreAlpha			(0, "Pre-alpha",		"pre");
TVersionPhase TVersionPhase::Alpha				(1, "Alpha",			"alpha");
TVersionPhase TVersionPhase::Beta				(2, "Beta",				"beta");
TVersionPhase TVersionPhase::ReleaseCandidate	(3, "Release Candidate","rc");
TVersionPhase TVersionPhase::Release			(4, "Release",			"r");

void TVersion::SetBuildText()
{
	BuildText			= String::Format("build %", sfi(Build));
	BuildPhaseText		= String::Format("build % %", sfi(Build), sfs(Phase->ShortName));
}

void TVersion::SetVersionText()
{
	VersionText			= String::Format("%.%.%",sfi(Major),sfi(Minor),sfi(SubMinor));
	VersionPhaseText	= String::Format("% %", sfs(VersionText), sfs(Phase->ShortName));
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

TModule::TModule( const String& _Name, const TVersion& _Version, const String& _Company )
{
	Name = _Name;
	Version = _Version;
	Company = _Company;
	SetIdentifyText();
}

TModule::TModule( const String& _Name, const TVersion& _Version )
{
	Name = _Name;
	Version = _Version;
	Company = String::Empty;
	SetIdentifyText();
}

void TModule::SetIdentifyText()
{
	IdentifyText = String::Format("% - % - %", sfs(Name), sfs(Version.VersionPhaseText), sfs(Company));
	IdentifyBuildText = String::Format("% - % - %", sfs(Name), sfs(Version.BuildPhaseText), sfs(Company));
}

TApplication Application;

#include "tplatform.h"

void TApplication::Begin( String _Name, const TVersion& _Version, String _Company )
{
	StartupPath = TPathDriver::CurrentDirectory();
	ExecutablePath = "Not Implemented";

	Name = _Name;
	Version = _Version;
	Company = _Company;

	SetIdentifyText();

	Platform.Initialize(); // TODO: move this to better place
}

void TApplication::Begin( String _Name, const TVersion& _Version )
{
	Begin( _Name,_Version, String::Empty );
}

void TApplication::PrintIdentifierToConsole(bool printModules)
{
	String str = String::Format("% (R) % Version %", sfs(Company), sfs(Name), sfs(Version.VersionPhaseText));
	Console.WriteLine(str);

	if (!printModules) return;

	Console.WriteHeader("Modules");

	for (ui32 i=0;i<Modules.Count;i++)
	{
		TModule* CurModule = Modules.Items[i];
		String str = String::Format("% (R) % Version %", sfs(CurModule->Company), sfs(CurModule->Name), sfs(CurModule->Version.VersionPhaseText));
		Console.WriteLine(str);
	}
}
