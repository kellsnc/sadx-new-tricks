#include "pch.h"

#define HammerScl pwp->free.f[7]

static bool EnableDoubleJump    = true;
static bool MovingGroundSpin    = true;
static bool InstantGroundSpin   = false;
static bool RemoveDizziness     = false;
static bool UpgradeRequired     = false;
static Buttons HammerPropButton = Buttons_X;

static float PropellerGravity            = 0.011f;
static float PropellerInitialAccTreshold = 1.0f;
static float PropellerInitialAcc         = 1.01f;
static float PropellerAirAccTreshold     = 7.0f;
static float PropellerAirAcc             = 1.005f;
static float DoubleJumpAcc               = 1.12f;
static float MovingGroundSpinAccel       = 0.025f;

static bool BlockDoubleJump[MaxPlayers]{};

static PL_ACTION DoubleJumpAnim = { nullptr, 78, 4, Anm_Amy_Jump, 1.12f, 1.0f };

TaskHook Amy_Exec_t(AmyRose);
FunctionHook<void, taskwk*, motionwk2*, playerwk*> Amy_RunActions_t((intptr_t)0x488880);


static void AmyDoubleJump(taskwk* twp, playerwk* pwp)
{
	auto pnum = TASKWK_PLAYERID(twp);

	if (EnableDoubleJump == true && CheckControl(pnum) && PressedButtons[pnum] & JumpButtons && BlockDoubleJump[pnum] == false && pwp->jumptimer > 8i16)
	{
		BlockDoubleJump[pnum] = true;
		EnemyBounceThing(pnum, 0, DoubleJumpAcc, 0);
		dsPlay_oneshot(1286, 0, 0, 0);
		pwp->mj.reqaction = 74;
	}
}

static bool CheckUpgrade(playerwk* pwp)
{
	return UpgradeRequired == false || pwp->equipment & Upgrades_LongHammer;
}

static void AmyProp_Physics(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
	auto RestoreGravity = pwp->p.weight;
	pwp->p.weight = PropellerGravity;

	PGetRotation(twp, mwp, pwp);
	PResetAngle(twp, mwp, pwp);
	PGetAcceleration(twp, mwp, pwp);
	PGetSpeed(twp, mwp, pwp);
	PSetPosition(twp, mwp, pwp);
	PresetPosition(twp, mwp, pwp);

	pwp->p.weight = RestoreGravity;
}

static void AmyProp_Run(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
	auto pnum = TASKWK_PLAYERID(twp);

	// If an object overrides the player action, stop
	if (Amy_RunNextAction(twp, mwp, pwp))
	{
		HammerScl = 0.0f;
		return;
	}

	// If the player stops holding the button, stop
	if (!(HeldButtons[pnum] & HammerPropButton))
	{
		twp->mode = Act_Amy_Fall;
		pwp->mj.reqaction = Anm_Amy_Fall;
		HammerScl = 0.0f;
		return;
	}

	// If the player touches the ground, stop
	if (twp->flag & STATUS_FLOOR)
	{
		dsPlay_oneshot(33, 0, 0, 0);
		twp->mode = Act_Amy_Walk;
		pwp->mj.reqaction = Anm_Amy_Stand;
		HammerScl = 0.0f;
		return;
	}

	// Initial acceleration if close to no input speed
	if (pwp->spd.x < PropellerInitialAccTreshold)
	{
		pwp->spd.x *= PropellerInitialAcc;
	}

	// Hammer Air acceleration
	if (pwp->spd.x < PropellerAirAccTreshold)
	{
		pwp->spd.x *= PropellerAirAcc;
	}

	// Fix velocity bug
	if (njScalor(&pwp->spd) == 0.0f)
	{
		pwp->spd.y -= 0.1f;
	}

	// Hammer Spin Animation
	pwp->mj.reqaction = Anm_Amy_HammerSpinAttack;

	// This is the hammer scale
	HammerScl = 1.2f;

	// Attack status
	twp->flag |= Status_Attack;
}

static void AmyProp_Check(taskwk* twp, playerwk* pwp)
{
	auto pnum = TASKWK_PLAYERID(twp);

	if (CheckUpgrade(pwp) && CheckControl(pnum) && PressedButtons[pnum] & HammerPropButton
		&& !(twp->flag & STATUS_FLOOR) && pwp->htp == nullptr)
	{
		twp->mode = Act_Amy_HammerProp;

		if (twp->ang.x || twp->ang.z)
		{
			PConvertVector_P2G(twp, &pwp->spd);
		}

		twp->ang.x = GravityAngle_X;
		twp->ang.z = GravityAngle_Z;

		if (pwp->spd.x >= 5.0f)
		{
			PlayVoice(1743);
		}
		else
		{
			dsPlay_oneshot(1279, 0, 0, 0);
		}
	}
}

static void AmyMovingSpin(taskwk* twp)
{
	if (HammerPropButton && !(twp->flag & STATUS_FLOOR))
	{
		twp->mode = Act_Amy_HammerProp;
	}
}

static void AmyMovingSpin_Physics(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
	if (pwp->mj.reqaction == Anm_Amy_HammerSpinAttack)
	{
		int curAnim = pwp->mj.reqaction;

		if (curAnim == Anm_Amy_HammerSpinStill || curAnim == Anm_Amy_HammerSpinTurn)
		{
			PGetRotation(twp, mwp, pwp);
		}

		PResetAngle(twp, mwp, pwp);
		PGetAcceleration(twp, mwp, pwp);
		PGetSpeed(twp, mwp, pwp);
		PSetPosition(twp, mwp, pwp);
		PresetPosition(twp, mwp, pwp);
	}
}

static void AmyInstantHammerSpin(taskwk* twp, playerwk* pwp)
{
	if (InstantGroundSpin && CheckUpgrade(pwp) && PressedButtons[TASKWK_PLAYERID(twp)] & HammerPropButton && pwp->spd.x > 0.5f)
	{
		pwp->free.sw[4] = 0; // reset dizziness timer
		dsPlay_oneshot(798, 0, 0, 0);
		twp->mode = Act_Amy_HammerSpin;
		pwp->mj.reqaction = 90; // hammer spin state is managed with animation IDs
	}
}

static void __cdecl Amy_RunActions_r(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
	switch (twp->mode)
	{
	case Act_Amy_Jump:
		AmyProp_Check(twp, pwp);
		AmyDoubleJump(twp, pwp);
		break;
	case Act_Amy_HammerJump:
	case Act_Amy_DashSpeedPanel:
	case Act_Amy_Spring:
	case Act_Amy_Fall:
	case Act_Amy_Launch:
		AmyProp_Check(twp, pwp);
		break;
	case Act_Amy_HammerSpin:
	case Act_Amy_SpinR:
		if (RemoveDizziness)
		{
			pwp->free.sw[4] = 0; // dizziness timer
		}

		AmyMovingSpin(twp);
		break;
	case Act_Amy_HammerProp:
		AmyProp_Run(twp, mwp, pwp);
		break;
	case Act_Amy_TailsGrab:
		if (Amy_RunNextAction(twp, mwp, pwp))
		{
			return;
		}

		TailsGrabAction((EntityData1*)twp, mwp, (CharObj2*)pwp, { 0.0f, -6.8f, 0.0f }, Anm_Amy_HangHook, Act_Amy_Fall, Anm_Amy_Fall);
		break;
	}

	Amy_RunActions_t.Original(twp, mwp, pwp);
}

static void __cdecl Amy_Exec_r(task* tsk)
{
	auto twp = tsk->twp; // main task info containing position, rotation, scale
	auto mwp = (motionwk2*)tsk->mwp; // contains movement information
	auto pwp = (playerwk*)mwp->work.ptr; // player-specific information (like physics, animation, etc.)

	if (EnableDoubleJump == true && twp->flag & STATUS_FLOOR)
	{
		BlockDoubleJump[TASKWK_PLAYERID(twp)] = false; // can double jump
	}

	switch (twp->mode)
	{
	case Act_Amy_Init:
		// Initialize the double jump animation
		DoubleJumpAnim.actptr = AmyAnimData[Anm_Amy_HammerSomerTrickA].actptr;
		AmyAnimData[74] = DoubleJumpAnim;
		break;
	case Act_Amy_Walk:
		AmyInstantHammerSpin(twp, pwp);
		break;
	case Act_Amy_HammerSpin:
		AmyMovingSpin_Physics(twp, mwp, pwp);
		break;
	case Act_Amy_HammerProp:
		AmyProp_Physics(twp, mwp, pwp);
		break;
	}

	Amy_Exec_t.Original(tsk);
}

void Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics)
{
	Amy_Exec_t.Hook(Amy_Exec_r);
	Amy_RunActions_t.Hook(Amy_RunActions_r);

	auto configgrp = config->getGroup("Amy");

	if (configgrp)
	{
		EnableDoubleJump  = configgrp->getBool("EnableDoubleJump", EnableDoubleJump);
		MovingGroundSpin  = configgrp->getBool("EnableMovingSpin", MovingGroundSpin);
		InstantGroundSpin = configgrp->getBool("InstantGroundSpin", InstantGroundSpin);
		RemoveDizziness   = configgrp->getBool("RemoveDizziness", RemoveDizziness);
		UpgradeRequired   = configgrp->getBool("UpgradeRequired", UpgradeRequired);
		HammerPropButton  = (Buttons)configgrp->getInt("HammerPropButton", HammerPropButton);
	}

	auto physgrp = physics->getGroup("Amy");

	if (physgrp)
	{
		PropellerGravity = physgrp->getFloat("PropellerGravity", PropellerGravity);
		PropellerInitialAccTreshold = physgrp->getFloat("PropellerInitialAccTreshold", PropellerInitialAccTreshold);
		PropellerInitialAcc = physgrp->getFloat("PropellerInitialAcc", PropellerInitialAcc);
		PropellerAirAccTreshold = physgrp->getFloat("PropellerAirAccTreshold", PropellerAirAccTreshold);
		PropellerAirAcc = physgrp->getFloat("PropellerAirAcc", PropellerAirAcc);
		DoubleJumpAcc = physgrp->getFloat("DoubleJumpAcc", DoubleJumpAcc);
		MovingGroundSpinAccel = physgrp->getFloat("MovingGroundSpinAccel", MovingGroundSpinAccel);
	}
}

