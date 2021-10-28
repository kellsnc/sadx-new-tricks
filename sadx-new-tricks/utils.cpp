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
		if (co2->field_A != 0)
		{
			return false;
		}
	}

	return !EV_MainThread_ptr && ControlEnabled == 1 && ControllerEnabled[id] == 1;
}