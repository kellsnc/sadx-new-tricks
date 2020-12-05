#include "pch.h"

void __cdecl Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config);

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

		Amy_Init(helperFunctions, config);

		delete config;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}