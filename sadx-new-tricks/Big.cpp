#include "pch.h"
#include "SADXModLoader.h"
#include "FunctionHook.h"
#include "UsercallFunctionHandler.h"
#include "IniFile.hpp"
#include "utils.h"
#include "Big.h"

static bool EnableFreeCursor = true;
static bool EnableJumpAttack = true;
static bool EnableAimCancel = true;

TaskHook BigTheCat_h(0x490A00);
UsercallFuncVoid(MoveFishingCursor, (task* tp), (tp), 0x46F850, rEAX);
UsercallFuncVoid(calcFishingCursorY, (Big_ypos* big_y_ptr, Float posx, Float posy, Float posz, Sint32 flag), (big_y_ptr, posx, posy, posz, flag), 0x46DA00, rESI, stack4, stack4, stack4, stack4);

static void __cdecl MoveFishingCursor_r(task* tp)
{
	auto twp = tp->twp;
	auto awp = tp->awp;
	auto pnum = TASKWK_PLAYERID(twp);
	auto ptwp = playertwp[pnum];

	auto& input = input_dataG[pnum];
	
	NJS_POINT3 v{};
	if (input.stroke != 0.0f)
	{
		v.x = njCos(input.angle) * 3.0f;
		v.z = njSin(input.angle) * 3.0f;
	}

	if (perG[pnum].on & Buttons_L)
	{
		v.y = +3.0f;
	}
	else if (perG[pnum].on & Buttons_R)
	{
		v.y = -3.0f;
	}

	NJS_POINT3 pos = { twp->pos.x, twp->pos.y - 15.0f, twp->pos.z };
	MSetPositionWIgnoreAttribute(&pos, &v, nullptr, ColFlags_Water, 1.0f);
	pos = { twp->pos.x, twp->pos.y - 20.0f, twp->pos.z };
	njAddVector(&pos, &v);

	// Limit position
	NJS_POINT3 center = { ptwp->pos.x - pos.x, ptwp->pos.y - pos.y, ptwp->pos.z - pos.z };
	Float limit = (playerpwp[pnum]->equipment & Upgrades_PowerRod ? 350.0f : 250.0f);
	if (njScalor(&center) > limit)
	{
		njUnitVector(&center);
		pos.x = ptwp->pos.x - center.x * limit;
		pos.y = ptwp->pos.y - center.y * limit;
		pos.z = ptwp->pos.z - center.z * limit;
	}

	// Additional height limit
	Float ylimit = njSin(0x4000) * (tp->awp->work.f[0] / 2);
	if (pos.y - ptwp->pos.y > ylimit)
	{
		pos.y = ptwp->pos.y + ylimit;
	}
	if (pos.y - ptwp->pos.y < -ylimit)
	{
		pos.y = ptwp->pos.y - ylimit;
	}

	twp->pos.x = pos.x;
	twp->pos.y = pos.y + 20.0f;
	twp->pos.z = pos.z;

	awp->work.f[1] = pos.y;
	awp->work.uw[6] = 0;
	awp->work.uw[7] = 0;

	v.x = twp->pos.x - ptwp->pos.x;
	v.z = twp->pos.z - ptwp->pos.z;
	awp->work.f[0] = njScalor(&v);
}

static void __cdecl BigTheCat_r(task* tp)
{
	auto twp = tp->twp;
	auto pwp = (playerwk*)tp->mwp->work.ptr;
	auto pnum = twp->counter.b[0];

	switch (twp->mode)
	{
	case MD_BIG_JUMP:
		if (EnableJumpAttack && perG[pnum].press & AttackButtons)
		{
			pwp->mj.reqaction = 49;
			twp->mode = MD_BIG_ATCK;
			twp->flag |= Status_Attack;
		}

		break;
	case MD_BIG_CAS1:
		if (EnableAimCancel && perG[pnum].press & JumpButtons)
		{
			CameraReleaseEventCamera();
			Big_Fish_Flag |= 0x4000;
			twp->mode = MD_BIG_STND;
		}

		break;
	}

	BigTheCat_h.Original(tp);
}

void Big_Init(const HelperFunctions& helperFunctions, const IniFile* config)
{
	BigTheCat_h.Hook(BigTheCat_r);

	auto configgrp = config->getGroup("Big");

	if (configgrp)
	{
		EnableFreeCursor = configgrp->getBool("EnableFreeCursor", EnableFreeCursor);
		EnableJumpAttack = configgrp->getBool("EnableJumpAttack", EnableJumpAttack);
		EnableAimCancel = configgrp->getBool("EnableAimCancel", EnableAimCancel);
	}

	if (EnableFreeCursor)
	{
		MoveFishingCursor.Hook(MoveFishingCursor_r);
	}
}
