#include "pch.h"

static bool EnableDoubleJump = true;
static bool MovingGroundSpin = true;
static Buttons HammerPropButton = Buttons_X;

static float PropellerGravity = 0.011f;
static float PropellerInitialAccTreshold = 1.0f;
static float PropellerInitialAcc = 1.01f;
static float PropellerAirAccTreshold = 7.0f;
static float PropellerAirAcc = 1.005f;
static float DoubleJumpAcc = 1.12f;
static float MovingGroundSpinAccel = 0.025f;

static bool BlockDoubleJump[MaxPlayers]{};

static AnimData DoubleJumpAnim = { nullptr, 78, 4, Anm_Amy_Jump, 1.12f, 1.0f };

static Trampoline* Amy_Exec_t = nullptr;
static Trampoline* Amy_RunActions_t = nullptr;

static void AmyDoubleJump(EntityData1* data, CharObj2* co2)
{
	if (EnableDoubleJump == true && CheckControl(data->CharIndex) && PressedButtons[data->CharIndex] & JumpButtons && BlockDoubleJump[data->CharIndex] == false && co2->JumpTime > 8)
	{
		BlockDoubleJump[data->CharIndex] = true;
		EnemyBounceThing(data->CharIndex, 0, DoubleJumpAcc, 0);
		PlaySound(1286, 0, 0, 0);
		co2->AnimationThing.Index = 74;
	}
}

static void AmyMovingSpin(EntityData1* data)
{
	if (HammerPropButton && !(data->Status & STATUS_FLOOR))
	{
		data->Action = Act_Amy_HammerProp;
	}
}

static void AmyMovingSpin_Physics(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	auto RestoreSpeed = co2->PhysicsData.GroundAccel;
	co2->PhysicsData.GroundAccel = MovingGroundSpinAccel;

	if (co2->AnimationThing.Index == Anm_Amy_HammerSpinAttack)
	{
		PlayerGetRotation((taskwk*)data, mwp, (playerwk*)co2);
		PlayerResetAngle((taskwk*)data, mwp, (playerwk*)co2);
		PlayerGetAcceleration((taskwk*)data, mwp, (playerwk*)co2);
		PlayerGetSpeed((taskwk*)data, mwp, (playerwk*)co2);
		PlayerSetPosition((taskwk*)data, mwp, (playerwk*)co2);
		PlayerUpdateSpeed((taskwk*)data, mwp, (playerwk*)co2);
	}

	co2->PhysicsData.GroundAccel = RestoreSpeed;
}

#pragma region Propeller
static void AmyProp_Physics(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	auto RestoreGravity = co2->PhysicsData.Gravity;
	co2->PhysicsData.Gravity = PropellerGravity;

	PlayerGetRotation((taskwk*)data, mwp, (playerwk*)co2);
	PlayerResetAngle((taskwk*)data, mwp, (playerwk*)co2);
	PlayerGetAcceleration((taskwk*)data, mwp, (playerwk*)co2);
	PlayerGetSpeed((taskwk*)data, mwp, (playerwk*)co2);
	PlayerSetPosition((taskwk*)data, mwp, (playerwk*)co2);
	PlayerUpdateSpeed((taskwk*)data, mwp, (playerwk*)co2);

	co2->PhysicsData.Gravity = RestoreGravity;
}

static void AmyProp_Run(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	// If an object overrides the player action, stop
	if (Amy_RunNextAction(co2, mwp, data))
	{
		co2->TailsFlightTime = 0.0f;
		return;
	}

	// If the player stops holding the button, stop
	if (!(HeldButtons[data->CharIndex] & HammerPropButton))
	{
		data->Action = Act_Amy_Fall;
		co2->AnimationThing.Index = Anm_Amy_Fall;
		co2->TailsFlightTime = 0.0f;
		return;
	}

	// If the player touches the ground, stop
	if (data->Status & STATUS_FLOOR)
	{
		PlaySound(33, 0, 0, 0);
		co2->AnimationThing.Index = Anm_Amy_Stand;
		data->Action = Act_Amy_Walk;
		co2->TailsFlightTime = 0.0f;
		return;
	}

	// Initial acceleration if close to no input speed
	if (co2->Speed.x < PropellerInitialAccTreshold)
	{
		co2->Speed.x *= PropellerInitialAcc;
	}

	// Hammer Air acceleration
	if (co2->Speed.x < PropellerAirAccTreshold)
	{
		co2->Speed.x *= PropellerAirAcc;
	}

	// Fix velocity bug
	if (njScalor(&co2->Speed) == 0)
	{
		co2->Speed.y -= 0.1f;
	}

	// Hammer Spin Animation
	co2->AnimationThing.Index = Anm_Amy_HammerSpinAttack;

	// This is the hammer scale
	co2->TailsFlightTime = 1.2f;

	// Attack status
	data->Status |= Status_Attack;
}

static inline void AmyProp_Check(EntityData1* data, CharObj2* co2)
{
	if (CheckControl(data->CharIndex) && PressedButtons[data->CharIndex] & HammerPropButton
		&& !(data->Status & STATUS_FLOOR) && co2->ObjectHeld == nullptr)
	{
		data->Action = Act_Amy_HammerProp;

		if (data->Rotation.x || data->Rotation.z)
		{
			PConvertVector_P2G((taskwk*)data, &co2->Speed);
		}

		data->Rotation.x = GravityAngle_X;
		data->Rotation.z = GravityAngle_Z;

		if (co2->Speed.x >= 5.0)
		{
			PlayVoice(1743);
		}
		else 
		{
			PlaySound(1279, 0, 0, 0);
		}
	}
}
#pragma endregion

static void __cdecl Amy_RunActions_r(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	switch (data->Action)
	{
	case Act_Amy_Jump:
		AmyProp_Check(data, co2);
		AmyDoubleJump(data, co2);
		break;
	case Act_Amy_HammerJump:
	case Act_Amy_DashSpeedPanel:
	case Act_Amy_Spring:
	case Act_Amy_Fall:
	case Act_Amy_Launch:
		AmyProp_Check(data, co2);
		break;
	case Act_Amy_HammerSpin:
		AmyMovingSpin(data);
		break;
	case Act_Amy_HammerProp:
		AmyProp_Run(data, mwp, co2);
		break;
	case Act_Amy_TailsGrab:
		if (Amy_RunNextAction(co2, mwp, data))
		{
			return;
		}

		TailsGrabAction(data, mwp, co2, { 0.0f, -6.8f, 0.0f }, Anm_Amy_HangHook, Act_Amy_Fall, Anm_Amy_Fall);
		break;
	}

	TRAMPOLINE(Amy_RunActions)(data, mwp, co2);
}

static void __cdecl Amy_Exec_r(task* tsk)
{
	auto data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	auto mwp = (motionwk2*)tsk->mwp; // task containing movement information
	auto co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	if (EnableDoubleJump == true && data->Status & STATUS_FLOOR)
	{
		BlockDoubleJump[data->CharIndex] = false; // can double jump
	}

	switch (data->Action)
	{
	case Act_Amy_Init:
		// Initialize the double jump animation
		DoubleJumpAnim.Animation = AmyAnimData[Anm_Amy_HammerSomerTrickA].Animation;
		AmyAnimData[74] = DoubleJumpAnim;
		break;
	case Act_Amy_HammerSpin:
		AmyMovingSpin_Physics(data, mwp, co2);
		break;
	case Act_Amy_HammerProp:
		AmyProp_Physics(data, mwp, co2);
		break;
	}

	TRAMPOLINE(Amy_Exec)(tsk);
}

void Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics)
{
	Amy_Exec_t = new Trampoline((int)Amy_Main, (int)Amy_Main + 0x7, Amy_Exec_r);
	Amy_RunActions_t = new Trampoline(0x488880, 0x488888, Amy_RunActions_r);

	auto configgrp = config->getGroup("Amy");

	if (configgrp)
	{
		EnableDoubleJump = configgrp->getBool("EnableDoubleJump", EnableDoubleJump);
		MovingGroundSpin = configgrp->getBool("EnableMovingSpin", MovingGroundSpin);
		HammerPropButton = (Buttons)configgrp->getInt("HammerPropButton", HammerPropButton);
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