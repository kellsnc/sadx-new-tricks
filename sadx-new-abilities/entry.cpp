#include "pch.h"

void __cdecl Sonic_Init(const HelperFunctions& helperFunctions, const IniFile* config);
void __cdecl Tails_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics);
void __cdecl Knuckles_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics);
void __cdecl Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics);
void __cdecl Tornado_Init(const HelperFunctions& helperFunctions, const IniFile* config);

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		const IniFile* physics = new IniFile(std::string(path) + "\\physics.ini");

		if (config->getBool("General", "EnableSonic", true)) {
			Sonic_Init(helperFunctions, config);
		}

		if (config->getBool("General", "EnableTails", true)) {
			Tails_Init(helperFunctions, config, physics);
		}
		
		if (config->getBool("General", "EnableKnuckles", true)) {
			Knuckles_Init(helperFunctions, config, physics);
		}

		if (config->getBool("General", "EnableAmy", true)) {
			Amy_Init(helperFunctions, config, physics);
		}
		
		if (config->getBool("General", "EnableTornado", true)) {
			Tornado_Init(helperFunctions, config);
		}
		
		delete physics;
		delete config;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}