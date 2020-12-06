#include "pch.h"

static bool EnableKnucklesSpinDash = true;

static float KnucklesSpinDashMaxInitialSpeed = 1.45f;
static float KnucklesSpinDashMaxSpeed = 8.0f;
static float KnucklesSpinDashSpeedIncrement = 0.28f;

Trampoline* Knuckles_Exec_t = nullptr;

void Knuckles_SpinDash(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	CommonSpinDash_Run(data, mwp, co2, KnucklesSpinDashMaxSpeed, KnucklesSpinDashSpeedIncrement, Anm_Knuckles_Roll, Act_Knuckles_Roll);
}

void Knuckles_CheckSpinDash(EntityData1* data, CharObj2* co2) {
	if (EnableKnucklesSpinDash == true) {
		CommonSpinDash_Check(data, co2, Anm_Knuckles_JumpOrSpin, Act_Knuckles_SpinDash, KnucklesSpinDashMaxInitialSpeed);
	}
}

void Knuckles_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	switch (data->Action) {
	case Act_Knuckles_Stand:
	case Act_Knuckles_Walk:
		Knuckles_CheckSpinDash(data, co2);
		break;
	case Act_Knuckles_TailsGrab:
		TailsGrabAction(data, mwp, co2, { 0.0f, -8.0f, -1.0f }, Anm_Knuckles_Hang, Act_Knuckles_Fall, Anm_Knuckles_Fall);
		break; 
	case Act_Knuckles_SpinDash:
		Knuckles_SpinDash(data, mwp, co2);
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

void __cdecl Knuckles_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics) {
	Knuckles_Exec_t = new Trampoline((int)Knuckles_Main, (int)Knuckles_Main + 0x8, Knuckles_Exec_r);

	EnableKnucklesSpinDash = config->getBool("Knuckles", "EnableKnucklesSpinDash", true);

	KnucklesSpinDashMaxInitialSpeed = physics->getFloat("Knuckles", "SpinDashMaxInitialSpeed", 1.45f);
	KnucklesSpinDashMaxSpeed = physics->getFloat("Knuckles", "SpinDashMaxSpeed", 8.0f);
	KnucklesSpinDashSpeedIncrement = physics->getFloat("Knuckles", "SpinDashSpeedIncrement", 0.28f);
}