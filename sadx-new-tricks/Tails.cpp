#include "pch.h"

static bool EnableTailsGrab = true;
static bool EnableTailsSpinDash = true;
static bool EnableTailsSpinTails = false;

static float TailsSpinDashMaxInitialSpeed = 1.25f;
static float TailsSpinDashMaxSpeed = 7.0f;
static float TailsSpinDashSpeedIncrement = 0.2f;

static Trampoline* Tails_Exec_t = nullptr;
static Trampoline* Tails_Render_t = nullptr;

static void SetPlayerGrabbed(EntityData1* data, EntityData1* player)
{
	Characters character = (Characters)player->CharID;

	// Launch the correct action per player because sadx is very arbitrary

	switch (character)
	{
	case Characters_Sonic:
		if (player->Action == Act_Sonic_Jump)
		{
			player->Action = Act_Sonic_TailsGrab;
		}
		else
		{
			return;
		}

		break;
	case Characters_Tails:
		if (player->Action == Act_Tails_Jump)
		{
			player->Action = Act_Tails_TailsGrab;
		}
		else
		{
			return;
		}

		break;
	case Characters_Knuckles:
		if (player->Action == Act_Knuckles_Jump)
		{
			player->Action = Act_Knuckles_TailsGrab;
		}
		else
		{
			return;
		}

		break;
	case Characters_Amy:
		if (player->Action == Act_Amy_Jump)
		{
			player->Action = Act_Amy_TailsGrab;
		}
		else
		{
			return;
		}

		break;
	default:
		return;
	}

	player->LoopData = (Loop*)data;
	player->Status = 0; // Remove eventual hurt flag for grabbed player
	data->field_A = 1; // Cannot grab another player
	data->Status = 0; // Remove eventual hurt flag for grabbing player
}

static void Tails_FlyGrab(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	// If Tails detaches the player
	if (data->field_A == 1 && PressedButtons[data->CharIndex] & Buttons_B)
	{
		data->field_A = 0; // TailsGrab flag here
	}

	if (data->field_A == 0 && ControllerEnabled[data->CharIndex] == true) // if can grab & not controlled by AI
	{
		// Tails AI checks
		if (data->CharIndex == 1 && TailsAI_ptr && (TailsAI_ptr->Data1->Action != 5))
		{
			return;
		}

		EntityData1* entity = GetCollidingEntityA(data);

		if (entity)
		{
			SetPlayerGrabbed(data, entity);
		}
	}
}

static void Tails_CheckSpinDash(EntityData1* data, CharObj2* co2)
{
	if (EnableTailsSpinDash == true)
	{
		CommonSpinDash_Check(data, co2, Anm_Tails_JumpOrSpin, Act_Tails_SpinDash, TailsSpinDashMaxInitialSpeed);
	}
}

static void Tails_SpinDash(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	if (Tails_RunNextAction(co2, mwp, data))
	{
		return;
	}

	CommonSpinDash_Run(data, mwp, co2, TailsSpinDashMaxSpeed, TailsSpinDashSpeedIncrement, Anm_Tails_Roll, Anm_Tails_Uncurl, Act_Tails_Roll);
}

static void Tails_TailsGrab(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	if (Tails_RunNextAction(co2, mwp, data))
	{
		data->LoopData = nullptr;
		co2->Powerups &= ~Powerups_Invincibility;
		return;
	}

	TailsGrabAction(data, mwp, co2, { 0.0f, -5.8f, 0.0f }, Anm_Tails_RockVertHang, Act_Tails_Fall, Anm_Tails_Fall);
}

static void Tails_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	if (data->Action != Act_Tails_Init)
	{
		// Note: data->field_A is capacity to hold player
		// Tails' Grab restore capacity to hold player
		if (data->field_A == 1 && data->Status & Status_Ground)
		{
			data->field_A = 0; // can grab a player
			co2->Powerups &= ~Powerups_Invincibility;
			co2->gap16[0] = 0;
		}
	}

	switch (data->Action)
	{
	case Act_Tails_Stand:
	case Act_Tails_Walk:
		Tails_CheckSpinDash(data, co2);

		break;
	case Act_Tails_Fly:
		Tails_FlyGrab(data, mwp, co2);
		break;
	case Act_Tails_TailsGrab:
		Tails_TailsGrab(data, mwp, co2);
		break;
	case Act_Tails_SpinDash:
		Tails_SpinDash(data, mwp, co2);
		break;
	}
}

static void Tails_Render_r(task* tsk)
{
	EntityData1* data = (EntityData1*)tsk->twp;
	motionwk* mwp = tsk->mwp;
	CharObj2* co2 = (CharObj2*)mwp->work.ptr;

	TRAMPOLINE(Tails_Render)(tsk);

	// Draw tails out of the ball if enabled in the config (otherwise the trampoline is not enabled)
	if (data->Action == Act_Tails_SpinDash || data->Action == Act_Tails_Roll)
	{
		njSetTexture(&MILES_TEXLIST);
		Direct3D_PerformLighting(2);
		njPushMatrixEx();
		njTranslateEx(&data->Position);
		njRotateZ_(data->Rotation.z);
		njRotateX_(data->Rotation.x);
		njRotateY_(-LOWORD(data->Rotation.y + 0x8000));
		njRotateZ(0, 0x500);
		njTranslate(0, data->Action == Act_Tails_SpinDash ? 2.2f : 4.0f, 5.0f, 0);
		njDrawModel_SADX(MILES_MODELS[9]);
		njDrawModel_SADX(MILES_MODELS[10]);
		Direct3D_PerformLighting(0);
		njPopMatrixEx();
	}
}

static void Tails_Exec_r(task* tsk)
{
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Tails_NewActions(data, mwp, co2);

	TRAMPOLINE(Tails_Exec)(tsk);
}

void Tails_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics)
{
	Tails_Exec_t = new Trampoline((int)Tails_Main, (int)Tails_Main + 0x7, Tails_Exec_r);

	if (config->getBool("Tails", "EnableTailsSpinTails", false))
	{
		Tails_Render_t = new Trampoline((int)Tails_Display, (int)Tails_Display + 0x8, Tails_Render_r);
	}

	EnableTailsGrab = config->getBool("Tails", "EnableTailsGrab", true);
	EnableTailsSpinDash = config->getBool("Tails", "EnableTailsSpinDash", true);

	TailsSpinDashMaxInitialSpeed = physics->getFloat("Tails", "SpinDashMaxInitialSpeed", 1.25f);
	TailsSpinDashMaxSpeed = physics->getFloat("Tails", "SpinDashMaxSpeed", 7.0f);
	TailsSpinDashSpeedIncrement = physics->getFloat("Tails", "SpinDashSpeedIncrement", 0.2f);
}