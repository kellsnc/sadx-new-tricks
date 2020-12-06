#include "pch.h"

static bool EnableTailsGrab = true;
static bool EnableTailsSpinDash = true;
static bool EnableTailsSpinTails = false;

static constexpr float TailsSpinDashMaxInitialSpeed = 1.25f;
static constexpr float TailsSpinDashMaxSpeed = 7.0f;
static constexpr float TailsSpinDashSpeedIncrement = 0.2f;

Trampoline* Tails_Exec_t = nullptr;
Trampoline* Tails_Render_t = nullptr;

void SetPlayerGrabbed(EntityData1* data, EntityData1* player) {
	Characters character = (Characters)player->CharID;

	// Launch the correct action per player because sadx is very arbitrary

	switch (character) {
	case Characters_Sonic:
		if (player->Action == Act_Sonic_Jump) {
			player->Action = Act_Sonic_TailsGrab;
		}
		else {
			return;
		}

		break;
	case Characters_Tails:
		if (player->Action == Act_Tails_Jump) {
			player->Action = Act_Tails_TailsGrab;
		}
		else {
			return;
		}

		break;
	case Characters_Knuckles:
		if (player->Action == Act_Knuckles_Jump) {
			player->Action = Act_Knuckles_TailsGrab;
		}
		else {
			return;
		}

		break;
	case Characters_Amy:
		if (player->Action == Act_Amy_Jump) {
			player->Action = Act_Amy_TailsGrab;
		}
		else {
			return;
		}

		break;
	default:
		return;
	}

	player->LoopData = (Loop*)data;
	player->Status = 0; // remove status when starting grab, prevent several issues
	data->field_A = 1; // cannot grab another player
}

void Tails_FlyGrab(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (data->field_A == 0 && ControllerEnabled[data->CharIndex] == true) { // if can grab & not controlled by AI
		
		for (int i = 0; i < MaxPlayers; ++i) {
			if (EntityData1Ptrs[i] != nullptr && i != data->CharIndex) {
				
				// Fix stupid hurt flag
				if (IsSpecificPlayerInSphere(&data->Position, 20.0f, i)) {
					co2->Powerups |= Powerups_Invincibility;
				}

				if (IsSpecificPlayerInSphere(&data->Position, 10.0f, i)) {
					SetPlayerGrabbed(data, EntityData1Ptrs[i]); // If a player is very close to flying tails
					break;
				}
			}
		}
	}
}

void Tails_SpinDash(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	CommonSpinDash_Run(data, mwp, co2, TailsSpinDashMaxSpeed, TailsSpinDashSpeedIncrement, Anm_Tails_Roll, Act_Tails_Roll);
}

void Tails_CheckSpinDash(EntityData1* data, CharObj2* co2) {
	if (EnableTailsSpinDash == true) {
		CommonSpinDash_Check(data, co2, Anm_Tails_JumpOrSpin, Act_Tails_SpinDash, TailsSpinDashMaxInitialSpeed);
	}
}

void Tails_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (data->Action != Act_Tails_Init) {

		// Note: data->field_A is capacity to hold player
		// Tails' Grab restore capacity to hold player
		if (data->field_A == 1 && data->Status & Status_Ground) {
			data->field_A = 0; // can grab a player
			co2->Powerups &= ~Powerups_Invincibility;
			co2->gap16[0] = 0;
		}
	}
	
	switch (data->Action) {
	case Act_Tails_Stand:
	case Act_Tails_Walk:
		Tails_CheckSpinDash(data, co2);
		break;
	case Act_Tails_Fly:
		Tails_FlyGrab(data, mwp, co2);
		break;
	case Act_Tails_TailsGrab:
		TailsGrabAction(data, mwp, co2, { 0.0f, -5.8f, 0.0f }, Anm_Tails_RockVertHang, Act_Tails_Fall, Anm_Tails_Fall);
		break;
	case Act_Tails_SpinDash:
		Tails_SpinDash(data, mwp, co2);
		break;
	}
}

void Tails_Render_r(task* tsk) {
	EntityData1* data = (EntityData1*)tsk->twp;
	motionwk* mwp = tsk->mwp;
	CharObj2* co2 = (CharObj2*)mwp->work.ptr;

	NonStaticFunctionPointer(void, Tails_Original, (task * tsk), Tails_Render_t->Target());
	Tails_Original(tsk);
	
	// Draw tails out of the ball
	if (EnableTailsSpinTails == true && (data->Action == Act_Tails_SpinDash || data->Action == Act_Tails_Roll)) {
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

void Tails_Exec_r(task* tsk) {
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Tails_NewActions(data, mwp, co2);

	NonStaticFunctionPointer(void, Tails_Original, (task * tsk), Tails_Exec_t->Target());
	Tails_Original(tsk);
}

void __cdecl Tails_Init(const HelperFunctions& helperFunctions, const IniFile* config) {
	Tails_Exec_t = new Trampoline((int)Tails_Main, (int)Tails_Main + 0x8, Tails_Exec_r);
	Tails_Render_t = new Trampoline((int)Tails_Display, (int)Tails_Display + 0x8, Tails_Render_r);

	EnableTailsGrab = config->getBool("Tails", "EnableTailsGrab", true);
	EnableTailsSpinDash = config->getBool("Tails", "EnableTailsSpinDash", true);
	EnableTailsSpinTails = config->getBool("Tails", "EnableTailsSpinTails", false);
}