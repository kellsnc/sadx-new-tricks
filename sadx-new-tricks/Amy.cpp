#include "pch.h"

static bool EnableDoubleJump = true;
static Buttons HammerPropButton = Buttons_X;

static float PropellerGravity = 0.011f;
static float PropellerInitialAccTreshold = 1.0f;
static float PropellerInitialAcc = 1.01f;
static float PropellerAirAccTreshold = 7.0f;
static float PropellerAirAcc = 1.005f;
static float DoubleJumpAcc = 1.12f;

AnimData DoubleJumpAnim = { nullptr, 78, 4, Anm_Amy_Jump, 1.12f, 1.0f };

Trampoline* Amy_Exec_t = nullptr;

void AmyDoubleJump(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (EnableDoubleJump == true && PressedButtons[data->CharIndex] & JumpButtons && co2->gap16[0] == 0 /* Can Double Jump */ && co2->JumpTime > 8) {
		co2->gap16[0] = 1;

		PlaySound(1286, 0, 0, 0);
		co2->Speed.y += DoubleJumpAcc;
		co2->Speed.x /= 4;
		co2->Speed.z /= 4;

		co2->AnimationThing.Index = 74;
	}
}

#pragma region Propeller
void AmyProp_Run(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	// If an object overrides the player action, stop
	if (Amy_RunNextAction(co2, mwp, data)) {
		co2->TailsFlightTime = 0.0f;
		return;
	}

	// If the player stops holding the button, stop
	if (!(HeldButtons[data->CharIndex] & HammerPropButton)) {
		data->Action = Act_Amy_Fall;
		co2->AnimationThing.Index = Anm_Amy_Fall;
		co2->TailsFlightTime = 0.0f;
		return;
	}

	// If the player touches the ground, stop
	if (data->Status & Status_Ground) {
		PlaySound(33, 0, 0, 0);
		co2->AnimationThing.Index = Anm_Amy_Stand;
		data->Action = Act_Amy_Walk;
		co2->TailsFlightTime = 0.0f;
		return;
	}
	
	// Initial acceleration if close to no input speed
	if (co2->Speed.x < PropellerInitialAccTreshold) {
		co2->Speed.x *= PropellerInitialAcc;
	}

	// Hammer Air acceleration
	if (co2->Speed.x < PropellerAirAccTreshold) {
		co2->Speed.x *= PropellerAirAcc;
	}

	// Fix velocity bug
	if (co2->Speed.x + co2->Speed.z + co2->Speed.y == 0) {
		co2->Speed.y -= 0.1f;
	}
	
	// Handle physics
	Float RestoreGravity = co2->PhysicsData.Gravity;
	co2->PhysicsData.Gravity = PropellerGravity;
	
	PlayerFunc_RotateToGravity(data, mwp, co2);
	PlayerFunc_Acceleration(data, mwp, co2);
	PlayerFunc_AnalogToDirection(data, mwp, co2);
	PlayerFunc_RunDynamicCollision(data, mwp, co2);
	PlayerFunc_UpdateSpeed(data, mwp, co2);
	
	co2->PhysicsData.Gravity = RestoreGravity;

	// Hammer Spin Animation
	co2->AnimationThing.Index = Anm_Amy_HammerSpinAttack;

	// This is the hammer scale
	co2->TailsFlightTime = 1.2f;

	// Attack status
	data->Status |= Status_Attack;
}

inline void AmyProp_Check(EntityData1* data, CharObj2* co2) {
	if (PressedButtons[data->CharIndex] & HammerPropButton && (data->Status & Status_Ground) != Status_Ground &&
		co2->field_A == 0 && co2->JumpTime > 5 && co2->ObjectHeld == nullptr) {

		data->Action = Act_Amy_HammerProp;

		if (data->Rotation.x || data->Rotation.z) {
			PlayerDirectionToVector(data, &co2->Speed);
		}

		data->Rotation.x = GravityAngle_X;
		data->Rotation.z = GravityAngle_Z;

		PlaySound(1279, 0, 0, 0);
	}
}
#pragma endregion

void Amy_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (EnableDoubleJump == true && data->Action != Act_Amy_Init && data->Status & Status_Ground) {
		co2->gap16[0] = 0; // can double jump
	}

	switch (data->Action) {
	case Act_Amy_Init:
		// Initialize the double jump animation
		DoubleJumpAnim.Animation = AmyAnimData[Anm_Amy_HammerSomerTrickA].Animation;
		AmyAnimData[74] = DoubleJumpAnim;

		break;
	case Act_Amy_Jump:
		AmyProp_Check(data, co2);
		AmyDoubleJump(data, mwp, co2);
		break;
	case Act_Amy_HammerJump:
	case Act_Amy_DashSpeedPanel:
		AmyProp_Check(data, co2);
		break;
	case Act_Amy_Spring:
	case Act_Amy_Launch:
		AmyProp_Check(data, co2);
		break;
	case Act_Amy_HammerProp:
		AmyProp_Run(data, mwp, co2);
		break;
	case Act_Amy_TailsGrab:
		if (Amy_RunNextAction(co2, mwp, data)) {
			return;
		}

		TailsGrabAction(data, mwp, co2, { 0.0f, -6.8f, 0.0f }, Anm_Amy_HangHook, Act_Amy_Fall, Anm_Amy_Fall);
		break;
	}
}

void Amy_Exec_r(task* tsk) {
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Amy_NewActions(data, mwp, co2);

	NonStaticFunctionPointer(void, Amy_Original, (task * tsk), Amy_Exec_t->Target());
	Amy_Original(tsk);
}

void Amy_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics) {
	Amy_Exec_t = new Trampoline((int)Amy_Main, (int)Amy_Main + 0x7, Amy_Exec_r);

	EnableDoubleJump = config->getBool("Amy", "EnableDoubleJump", true);
	HammerPropButton = (Buttons)config->getInt("Amy", "HammerPropButton", Buttons_X);

	PropellerGravity = physics->getFloat("Amy", "PropellerGravity", 0.011f);
	PropellerInitialAccTreshold = physics->getFloat("Amy", "PropellerInitialAccTreshold", 1.0f);
	PropellerInitialAcc = physics->getFloat("Amy", "PropellerInitialAcc", 1.01f);
	PropellerAirAccTreshold = physics->getFloat("Amy", "PropellerAirAccTreshold", 7.0f);
	PropellerAirAcc = physics->getFloat("Amy", "PropellerAirAcc", 1.005f);
	DoubleJumpAcc = physics->getFloat("Amy", "DoubleJumpAcc", 1.12f);
}