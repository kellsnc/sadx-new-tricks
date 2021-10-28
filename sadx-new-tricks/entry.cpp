#include "pch.h"

void Sonic_Init(const HelperFunctions& helperFunctions, const IniFile* config);
void Tails_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics);
void Knuckles_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics);
void Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics);
void Tornado_Init(const HelperFunctions& helperFunctions, const IniFile* config);

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const auto config = new IniFile(std::string(path) + "\\config.ini");
		const auto physics = new IniFile(std::string(path) + "\\physics.ini");

		auto configgrp = config->getGroup("General");

		if (configgrp->getBool("EnableSonic", true))
		{
			Sonic_Init(helperFunctions, config);
		}

		if (configgrp->getBool("EnableTails", true))
		{
			Tails_Init(helperFunctions, config, physics);
		}
		
		if (configgrp->getBool("EnableKnuckles", true))
		{
			Knuckles_Init(helperFunctions, config, physics);
		}

		if (configgrp->getBool("EnableAmy", true))
		{
			Amy_Init(helperFunctions, config, physics);
		}
		
		if (configgrp->getBool("EnableTornado", true))
		{
			Tornado_Init(helperFunctions, config);
		}
		
		delete physics;
		delete config;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}