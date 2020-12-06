#include "pch.h"

void __cdecl Sonic_Init(const HelperFunctions& helperFunctions, const IniFile* config);
void __cdecl Tails_Init(const HelperFunctions& helperFunctions, const IniFile* config);
void __cdecl Knuckles_Init(const HelperFunctions& helperFunctions, const IniFile* config);
void __cdecl Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config);
void __cdecl Tornado_Init(const HelperFunctions& helperFunctions, const IniFile* config);

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

		Sonic_Init(helperFunctions, config);
		Tails_Init(helperFunctions, config);
		Knuckles_Init(helperFunctions, config);
		Amy_Init(helperFunctions, config);
		Tornado_Init(helperFunctions, config);

		delete config;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}