#include "pch.h"
#include "SADXModLoader.h"
#include "FunctionHook.h"
#include "UsercallFunctionHandler.h"
#include "IniFile.hpp"
#include "utils.h"
#include "Big.h"

static bool EnableFreeCursor = true;

TaskHook BigTheCat_h(0x490A00);
UsercallFuncVoid(MoveFishingCursor, (task* tp), (tp), 0x46F850, rEAX);

static Float calcFishingCursorY_custom(Float posx, Float posy, Float posz, task* tp)
{
	auto pnum = TASKWK_PLAYERID(tp->twp);
	auto ptwp = playertwp[pnum];
	auto& ang = tp->twp->value.w[0];

	Angle old_ang = ang;
	Angle limit = (playerpwp[pnum]->equipment & Upgrades_PowerRod ? 0x4000 : 0x2000);

	if (perG[pnum].on & Buttons_L)
	{
		ang = min(+limit, ang + 0x100);
	}
	
	if (perG[pnum].on & Buttons_R)
	{
		ang = max(-limit, ang - 0x100);
	}

	Float new_y = ptwp->pos.y + (njSin(ang) * (tp->awp->work.f[0] / 2));

	posy -= 20.0f;
	NJS_POINT3 pos = { posx, posy + 2.0f, posz };
	NJS_POINT3 dir = { 0.0f, new_y - posy, 0.0f };

	if (1 /*!MSetPosition(&pos, &dir, NULL, 1.0f)*/)
	{
		return new_y;
	}
	else
	{
		ang = old_ang;
		return posy;
	}
}

static bool chkCursorLimit_custom(Float pos_x, Float pos_y, Float pos_z, Uint32 pnum)
{
	auto ptwp = playertwp[pnum];
	Float dist = njSqrt((ptwp->pos.x - pos_x) * (ptwp->pos.x - pos_x) + (ptwp->pos.z - pos_z) * (ptwp->pos.z - pos_z));
	return dist > (playerpwp[pnum]->equipment& Upgrades_PowerRod ? 350.0f : 250.0f);
}

static void __cdecl MoveFishingCursor_r(task* tp)
{
	auto twp = tp->twp;
	auto awp = tp->awp;
	auto pnum = TASKWK_PLAYERID(twp);
	auto ptwp = playertwp[pnum];

	auto& input = input_dataG[pnum];
	
	NJS_POINT3 v;
	Angle3 ang;
	if (input.stroke == 0.0f)
	{
		v.x = 0.0f;
		v.y = 0.0f;
		v.z = 0.0f;
		ang = twp->ang; // added
	}
	else
	{
		v.x = njCos(input.angle) * 3.0f;
		v.y = 0.0f;
		v.z = njSin(input.angle) * 3.0f;
		
		/*MSetPosition(&pos, &v, &ang, 3.0f);*/
	}

	Float x = twp->pos.x + v.x;
	Float y = twp->pos.y + v.y;
	Float z = twp->pos.z + v.z;

	Float h = calcFishingCursorY_custom(x, y, z, tp);

	if (!chkCursorLimit_custom(x, h, z, pnum))
	{
		twp->pos.x = x;
		twp->pos.y = h + 20.0f;
		twp->pos.z = z;

		awp->work.f[1] = h;
		awp->work.uw[6] = 0;
		awp->work.uw[7] = 0;
	}

	v.x = twp->pos.x - ptwp->pos.x;
	v.z = twp->pos.z - ptwp->pos.z;
	awp->work.f[0] = njScalor(&v);
}

static void __cdecl BigTheCat_r(task* tp)
{
	BigTheCat_h.Original(tp);
}

void Big_Init(const HelperFunctions& helperFunctions, const IniFile* config)
{
	//BigTheCat_h.Hook(BigTheCat_r);

	auto configgrp = config->getGroup("Big");

	if (configgrp)
	{
		EnableFreeCursor = configgrp->getBool("EnableDoubleJump", EnableFreeCursor);
	}

	if (EnableFreeCursor)
	{
		MoveFishingCursor.Hook(MoveFishingCursor_r);
	}
}
