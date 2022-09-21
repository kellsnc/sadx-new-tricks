#include "pch.h"

TaskHook Sonic_Exec_t(SonicTheHedgehog);
FunctionHook<void, taskwk*, motionwk2*, playerwk*> Sonic_RunActions_t((intptr_t)Sonic_Act1);

static Buttons InstantLightDashButton = Buttons_Y;

static void CheckRunInstantDash(EntityData1* data, CharObj2* co2)
{
	if (InstantLightDashButton && PressedButtons[data->CharIndex] & InstantLightDashButton &&
		HomingAttackTarget_Sonic_B_Index > 0 && co2->Upgrades & 3)
	{
		auto distance = 10000000.0f;
		auto closest = &HomingAttackTarget_Sonic_B; // Target rings

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

static void SonicInstantDash(EntityData1* data, motionwk2* mwp, CharObj2* co2)
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
	}
}

static void Sonic_NewActions(EntityData1* data, motionwk2* mwp, CharObj2* co2)
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

void __cdecl Sonic_RunActions_r(taskwk* data, motionwk2* mwp, playerwk* co2)
{
	Sonic_NewActions((EntityData1*)data, mwp, (CharObj2*)co2);

	Sonic_RunActions_t.Original(data, mwp, co2);
}

void Sonic_Exec_r(task* tsk)
{
	auto data = tsk->twp;
	auto mwp = (motionwk2*)tsk->mwp; // task containing movement information
	auto co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	if (data->mode == Act_Sonic_InstantDash)
	{
		Sonic_InitLightDash((EntityData1*)data, (EntityData2*)mwp, co2);

		if (PSetPosition(data, mwp, (playerwk*)co2) == 2) // colliding with wall
		{
			KnockSonic((EntityData1*)data, co2);
			co2->LightdashTime = 0;
		}
		else
		{
			PresetPosition((taskwk*)data, mwp, (playerwk*)co2);
			co2->SonicSpinTimeProbably |= 1u;
		}
	}

	Sonic_Exec_t.Original(tsk);
}

void Sonic_Init(const HelperFunctions& helperFunctions, const IniFile* config)
{
	InstantLightDashButton = (Buttons)config->getInt("Sonic", "InstantLSD", InstantLightDashButton);

	if (InstantLightDashButton) 
	{
		Sonic_Exec_t.Hook(Sonic_Exec_r);
		Sonic_RunActions_t.Hook(Sonic_RunActions_r);
	}
}