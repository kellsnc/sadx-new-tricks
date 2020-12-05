#include "pch.h"

static bool EnableTailsGrab = true;

Trampoline* Tails_Exec_t = nullptr;

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
	if (co2->Speed.x > 0) {
		co2->Speed.x -= 0.1f;
	}

	++co2->SonicSpinTimer;

	if (co2->SonicSpinTimer < 300) {
		if (HeldButtons[data->CharIndex] & Buttons_X) {
			if (co2->SpindashSpeed < 7.0) {
				co2->SpindashSpeed += 0.2f;
			}
		}
		else {
			DoSoundQueueThing(767);
			DoSoundQueueThing(763);
			data->Action = Act_Tails_Roll;
			co2->AnimationThing.Index = Anm_Tails_Roll;
			co2->Speed.x = co2->SpindashSpeed;
			co2->SpindashSpeed = 0;
			co2->SonicSpinTimer = 0;
		}
	}
	else {
		DoSoundQueueThing(767);
		DoSoundQueueThing(763);
		data->Action = Act_Tails_Stand;
		co2->IdleTime = 0;
		data->Status &= ~(Status_Ball | Status_Attack);
	}
	
	PlayerFunc_Move(data, mwp, co2);
	PlayerFunc_Acceleration(data, mwp, co2);
	PlayerFunc_AnalogToDirection(data, mwp, co2);
	PlayerFunc_RunDynamicCollision(data, mwp, co2);
	PlayerFunc_UpdateSpeed(data, mwp, co2);
}

void Tails_CheckSpinDash(EntityData1* data, CharObj2* co2) {
	if (PressedButtons[data->CharIndex] & Buttons_X) {
		co2->AnimationThing.Index = Anm_Tails_JumpOrSpin;
		data->Status |= Status_Attack | Status_Ball;
		data->Action = Act_Tails_SpinDash;
		co2->SpindashSpeed = fmin(1.25f, co2->Speed.x);
		QueueSound_DualEntity(767, data, 1, 0, 2);
		co2->SonicSpinTimer = 0;
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

	EnableTailsGrab = config->getBool("Tails", "EnableTailsGrab", true);
}