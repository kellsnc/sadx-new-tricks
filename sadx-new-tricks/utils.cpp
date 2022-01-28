#include "pch.h"

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions)
{
	std::string fullPath = "system\\";
	fullPath = fullPath + name + ".saanim";

	AnimationFile* anm = new AnimationFile(helperFunctions.GetReplaceablePath(fullPath.c_str()));

	if (anm->getmodelcount() == 0)
	{
		delete anm;
		*info = nullptr;
	}
	else
	{
		*info = anm;
	}
};

bool CheckControl(int id)
{
	auto co2 = CharObj2Ptrs[id];

	if (co2)
	{
		if (co2->NoControlTime != 0)
		{
			return false;
		}
	}

	return !EV_MainThread_ptr && ControlEnabled == 1 && ControllerEnabled[id] == 1;
}

void __cdecl PlayerClearSpeed(EntityData2* a1, CharObj2* a2)
{
	if (a2)
	{
		a2->Speed.z = 0.0;
		a2->Speed.y = 0.0;
		a2->Speed.x = 0.0;
	}
	if (a1)
	{
		a1->VelocityDirection.z = 0.0;
		a1->VelocityDirection.y = 0.0;
		a1->VelocityDirection.x = 0.0;
	}
}