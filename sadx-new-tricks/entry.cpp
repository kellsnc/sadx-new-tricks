#include "pch.h"
#include "SADXModLoader.h"
#include "IniFile.hpp"

#include "Sonic.h"
#include "Tails.h"
#include "Knuckles.h"
#include "Amy.h"
#include "Tornado.h"

static bool EnableSonic = true;
static bool EnableTails = true;
static bool EnableKnuckles = true;
static bool EnableAmy = true;
static bool EnableTornado = false;

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const auto config = new IniFile(std::string(path) + "\\config.ini");
		const auto physics = new IniFile(std::string(path) + "\\physics.ini");

		IniGroup* configgrp;
		if (configgrp = config->getGroup("General"))
		{
			EnableSonic = configgrp->getBool("EnableSonic", EnableSonic);
			EnableTails = configgrp->getBool("EnableTails", EnableTails);
			EnableKnuckles = configgrp->getBool("EnableKnuckles", EnableKnuckles);
			EnableAmy = configgrp->getBool("EnableAmy", EnableAmy);
			EnableTornado = configgrp->getBool("EnableTornado", EnableTornado);
		}

		if (EnableSonic)
		{
			Sonic_Init(helperFunctions, config);
		}

		if (EnableTails)
		{
			Tails_Init(helperFunctions, config, physics);
		}
		
		if (EnableKnuckles)
		{
			Knuckles_Init(helperFunctions, config, physics);
		}

		if (EnableAmy)
		{
			Amy_Init(helperFunctions, config, physics);
		}
		
		if (EnableTornado)
		{
			Tornado_Init(helperFunctions, config);
		}
		
		delete physics;
		delete config;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}
