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

void PlayerClearSpeed(motionwk2* mwp, playerwk* pwp)
{
	if (pwp)
	{
		pwp->spd.z = 0.0;
		pwp->spd.y = 0.0;
		pwp->spd.x = 0.0;
	}

	if (mwp)
	{
		mwp->spd.z = 0.0;
		mwp->spd.y = 0.0;
		mwp->spd.x = 0.0;
	}
}

bool isSuper(playerwk* pwp)
{
	return pwp->equipment & Upgrades_SuperSonic;
}

bool isSuper(int pnum)
{
	return playerpwp[pnum] && isSuper(playerpwp[pnum]);
}

bool isTailsAI(int pnum)
{
	return playertwp[pnum] && TASKWK_CHARID(playertwp[pnum]) == Characters_Tails && TailsAI_ptr;
}