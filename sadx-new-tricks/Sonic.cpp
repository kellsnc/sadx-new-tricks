#include "pch.h"

static Trampoline* Sonic_Exec_t = nullptr;

static Buttons InstantLightDashButton = Buttons_Y;

static void CheckRunInstantDash(EntityData1* data, CharObj2* co2)
{
	if (InstantLightDashButton && PressedButtons[data->CharIndex] & InstantLightDashButton &&
		HomingAttackTarget_Sonic_B_Index > 0 && co2->Upgrades & Upgrades_CrystalRing)
	{
		float distance = 10000000.0f;
		HomingAttackTarget* closest = HomingAttackTarget_Sonic_B; // Target rings

		for (int i = 0; i < HomingAttackTarget_Sonic_B_Index; ++i)
		{
			if (HomingAttackTarget_Sonic_B[i].distance < distance)
			{
				distance = HomingAttackTarget_Sonic_B[i].distance;
				closest = &HomingAttackTarget_Sonic_B[i];
			}
		}

		if (distance < 1500.0f)
		{
			data->Status &= ~Status_Ball | Status_Attack;
			data->Action = Act_Sonic_InstantDash;
			co2->AnimationThing.Index = Anm_Sonic_LightDashFlight;
			co2->LightdashTime = 10;
			co2->LightdashTimer = 0;
		}
	}
}

static void KnockSonic(EntityData1* data, CharObj2* co2)
{
	if (!MetalSonicFlag)
	{
		PlaySound(1222, 0, 0, 0);
	}

	data->Status &= ~(Status_Unknown6 | Status_DisableControl | Status_OnPath | Status_DoNextAction | Status_HoldObject | Status_LightDash | Status_Ball);
	data->Action = Act_Sonic_Knocked;
	co2->AnimationThing.Index = Anm_Sonic_HitWall;
	DoSoundQueueThing(764);

	NonStaticFunctionPointer(void, sub_440790, (EntityData1*), 0x440790);
	sub_440790(data);
}

static void SonicInstantDash(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	if (Sonic_NAct(co2, data, (EntityData2*)mwp))
	{
		co2->LightdashTime = 0;
		DoSoundQueueThing(764);
		data->Status &= ~Status_Attack;
	}
	else
	{
		if ((data->CollisionInfo->CollidingObject && data->CollisionInfo->CollidingObject->Object->Data1 && data->CollisionInfo->CollidingObject->Object->Data1->Unknown == 11))
		{
			co2->LightdashTimer = 0;
		}

		data->Status |= Status_Attack;

		if (--co2->LightdashTime <= 0 || ++co2->LightdashTimer > 100)
		{
			data->Action = Act_Sonic_Brake;
			co2->AnimationThing.Index = Anm_Sonic_Brake;
			data->Status &= ~Status_Attack;
			co2->LightdashTime = 0;
			DoSoundQueueThing(764);
		}

		Sonic_InitLightDash(data, (EntityData2*)mwp, co2);
		int type = PlayerFunc_RunDynamicCollision(data, mwp, co2);

		if (type == 2)
		{ // colliding with wall
			KnockSonic(data, co2);
			co2->LightdashTime = 0;
		}
		else
		{
			PlayerFunc_UpdateSpeed(data, mwp, co2);
			co2->SonicSpinTimeProbably |= 1u;
		}
	}
}

static void Sonic_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	switch (data->Action)
	{
	case Act_Sonic_Walk:
	case Act_Sonic_Stand:
	case Act_Sonic_Jump:
		CheckRunInstantDash(data, co2);
		break;
	case Act_Sonic_TailsGrab:
		TailsGrabAction(data, mwp, co2, { -0.5f, -7.5f, 0.0f }, Anm_Sonic_Hang, Act_Sonic_Fall, Anm_Sonic_Fall);
		break;
	case Act_Sonic_InstantDash:
		SonicInstantDash(data, mwp, co2);
		break;
	}
}

static void Sonic_Exec_r(task* tsk)
{
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Sonic_NewActions(data, mwp, co2);

	TRAMPOLINE(Sonic_Exec)(tsk);
}

void Sonic_Init(const HelperFunctions& helperFunctions, const IniFile* config)
{
	Sonic_Exec_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x7, Sonic_Exec_r);

	InstantLightDashButton = (Buttons)config->getInt("Sonic", "InstantLSD", 0);
}