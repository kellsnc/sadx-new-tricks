#include "pch.h"

Trampoline* Knuckles_Exec_t = nullptr;

void Knuckles_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	switch (data->Action) {
	case Act_Knuckles_TailsGrab:
		TailsGrabAction(data, mwp, co2, { 0.0f, -8.0f, -1.0f }, Anm_Knuckles_Hang, Act_Knuckles_Fall, Anm_Knuckles_Fall);
		break;
	}
}

void Knuckles_Exec_r(task* tsk) {
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Knuckles_NewActions(data, mwp, co2);

	NonStaticFunctionPointer(void, Knuckles_Original, (task * tsk), Knuckles_Exec_t->Target());
	Knuckles_Original(tsk);
}

void __cdecl Knuckles_Init(const HelperFunctions& helperFunctions, const IniFile* config) {
	Knuckles_Exec_t = new Trampoline((int)Knuckles_Main, (int)Knuckles_Main + 0x8, Knuckles_Exec_r);
}