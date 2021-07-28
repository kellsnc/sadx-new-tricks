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

float GetLength(NJS_VECTOR* orig, NJS_VECTOR* dest)
{
	return powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2);
}

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest)
{
	return sqrtf(GetLength(orig, dest));
}

bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius)
{
	return GetDistance(center, pos) <= radius;
}

int IsPlayerInsideSphere_(NJS_VECTOR* center, float radius)
{
	for (uint8_t player = 0; player < MaxPlayers; ++player)
	{
		if (!EntityData1Ptrs[player]) continue;

		if (IsPointInsideSphere(center, &EntityData1Ptrs[player]->Position, radius))
		{
			return player + 1;
		}
	}

	return 0;
}

int IsPlayerInsideSphere_(float x, float y, float z, float radius)
{
	NJS_VECTOR vec =
	{ x, y, z };
	return IsPlayerInsideSphere_(&vec, radius);
}

bool IsSpecificPlayerInSphere(float x, float y, float z, float radius, int player)
{
	NJS_VECTOR vec =
	{ x, y, z };
	return IsPointInsideSphere(&vec, &EntityData1Ptrs[player]->Position, radius);
}

bool IsSpecificPlayerInSphere(NJS_VECTOR* center, float radius, int player)
{
	return IsPointInsideSphere(center, &EntityData1Ptrs[player]->Position, radius);
};