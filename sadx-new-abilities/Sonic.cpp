#include "pch.h"

Trampoline* Sonic_Exec_t = nullptr;

void Sonic_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	switch (data->Action) {
	case Act_Sonic_TailsGrab:
		TailsGrabAction(data, mwp, co2, { -0.5f, -7.5f, 0.0f }, Anm_Sonic_Hang, Act_Sonic_Fall, Anm_Sonic_Fall);
		break;
	}
}

void Sonic_Exec_r(task* tsk) {
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Sonic_NewActions(data, mwp, co2);

	NonStaticFunctionPointer(void, Sonic_Original, (task * tsk), Sonic_Exec_t->Target());
	Sonic_Original(tsk);
}

void __cdecl Sonic_Init(const HelperFunctions& helperFunctions, const IniFile* config) {
	Sonic_Exec_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x8, Sonic_Exec_r);
}