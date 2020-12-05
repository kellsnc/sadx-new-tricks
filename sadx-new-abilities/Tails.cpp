#include "pch.h"

static bool EnableTailsGrab = true;

Trampoline* Tails_Exec_t = nullptr;

void SetPlayerGrabbed(EntityData1* data, EntityData1* player) {
	Characters character = (Characters)player->CharID;

	switch (character) {
	case Characters_Amy:
		if (player->Action == Act_Amy_Jump) {
			player->Action = Act_Amy_TailsGrab;
		}
		else {
			return;
		}
		
		break;
	}

	player->LoopData = (Loop*)data;
	data->field_A = 1; // cannot grab another player
}

void Tails_FlyGrab(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (data->field_A == 0 && ControllerEnabled[data->CharIndex] == true) {
		
		for (int i = 0; i < MaxPlayers; ++i) {
			if (i != data->CharIndex && EntityData1Ptrs[i] != nullptr)

			if (IsSpecificPlayerInSphere(&data->Position, 10.0f, i)) {
				SetPlayerGrabbed(data, EntityData1Ptrs[i]);
				break;
			}
		}
	}
}

void Tails_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (data->Action != Act_Tails_Init && data->field_A == 1 && data->Status & Status_Ground) {
		data->field_A = 0; // can grab a player
	}
	
	switch (data->Action) {
	case Act_Tails_Fly:
		co2->Speed.y = 0;
		Tails_FlyGrab(data, mwp, co2);
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