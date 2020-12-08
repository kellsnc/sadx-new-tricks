#include "pch.h"

static bool EnableKnucklesSpinDash = true;
static bool EnableKnucklesDrillClaw = true;
static bool EnableKnucklesWallDig = true;
static bool DiggableRMWalls = true;

static float KnucklesSpinDashMaxInitialSpeed = 1.45f;
static float KnucklesSpinDashMaxSpeed = 8.0f;
static float KnucklesSpinDashSpeedIncrement = 0.28f;
static float KnucklesDrillSpeed = 7.0f;

Trampoline* Knuckles_Exec_t = nullptr;

AnimData DrillClawAnim = { nullptr, 78, 3, Anm_Knuckles_CustomDrillClaw, 0.5f, 2.0f };
AnimData DrillDigAnim = { nullptr, 78, 4, Anm_Knuckles_Dig, 1.0f, 1.5f };

AnimationFile* DrillClawMotion = nullptr;

void Knuckles_AfterImages(task* tsk) {
	if (!MissedFrames) {
		CharObj2* co2 = CharObj2Ptrs[tsk->twp->value.b[0]];
		taskwk* wk = tsk->twp;

		if (IsGamePaused() == false) {
			wk->scl.x -= 0.1f;
			
			if (wk->scl.x <= 0.0f) {
				FreeTask(tsk);
				return;
			}
		}

		if (wk->scl.x < 0.9f) {
			Direct3D_PerformLighting(2);
			njSetTexture(&KNUCKLES_TEXLIST);
			BackupConstantAttr();

			njControl3D_Backup();
			njControl3D_Add(NJD_CONTROL_3D_OFFSET_MATERIAL);
			njControl3D_Add(NJD_CONTROL_3D_CONSTANT_MATERIAL);

			AddConstantAttr(0, NJD_FLAG_USE_ALPHA);
			njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
			njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
			SetMaterialAndSpriteColor_Float(wk->scl.x, 1.0f, 1.0f, 1.0f);
			
			njPushMatrixEx();
			njTranslateEx(&wk->pos);
			njRotateZ_(wk->ang.z);
			njRotateX_(wk->ang.x);
			njRotateY_(-0x8000 - LOWORD(wk->ang.y));
			njAction_Queue(DrillClawAnim.Animation, wk->scl.y, (QueuedModelFlagsB)0);
			njPopMatrixEx();

			njControl3D_Restore();
			njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
			njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
			ClampGlobalColorThing_Thing();
			RestoreConstantAttr();
			Direct3D_PerformLighting(0);
		}
	}
}

void LoadKnucklesAfterImages(EntityData1* data, CharObj2* co2) {
	task* tsk = CreateElementalTask(LoadObj_Data1, tasklevel::LEV_4, Knuckles_AfterImages);
	tsk->disp = Knuckles_AfterImages;
	
	taskwk* wk = tsk->twp;

	wk->value.b[0] = data->CharIndex;
	wk->scl.x = 1.0f;
	wk->scl.y = co2->AnimationThing.Frame;
	wk->pos = data->Position;
	wk->ang = *(Angle3*)&data->Rotation;
}

void Knuckles_CheckSpinDash(EntityData1* data, CharObj2* co2) {
	if (EnableKnucklesSpinDash == true) {
		CommonSpinDash_Check(data, co2, Anm_Knuckles_JumpOrSpin, Act_Knuckles_SpinDash, KnucklesSpinDashMaxInitialSpeed);
	}
}

void Knuckles_CheckDrillClaw(EntityData1* data, CharObj2* co2) {
	if (EnableKnucklesDrillClaw == true && HeldButtons[data->CharIndex] & Buttons_B) {
		data->Action = Act_Knuckles_DrillClaw;
		data->Status &= ~Status_Ball;
		LoadKnucklesAfterImages(data, co2);
	}
}

void Knuckles_CheckWallDig(EntityData1* data, CharObj2* co2) {
	if (co2->Upgrades & Upgrades_ShovelClaw && PressedButtons[data->CharIndex] & AttackButtons && ((CurrentLevel == LevelIDs_RedMountain && CurrentAct == 2 && DiggableRMWalls) || (co2->SurfaceFlags & ColFlags_Dig))) {
		data->Action = Act_Knuckles_Dig;
		data->field_A = Act_Knuckles_Climb; // store next action after walldig in this
		co2->AnimationThing.Index = Anm_Knuckles_CustomDrillDig;
		co2->AnimationThing.Frame = 0.2f;
	}
}

void Knuckles_DrillClaw(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (Knuckles_RunNextAction(co2, mwp, data)) {
		return;
	}
	
	// Stop if B is not held anymore
	if ((HeldButtons[data->CharIndex] & Buttons_B) != Buttons_B) {
		data->Status |= Status_Ball;
		data->Action = Act_Knuckles_Jump;
		co2->AnimationThing.Index = Anm_Knuckles_GlideCancelRoll;
		return;
	}

	// Stop if touches the ground
	if (data->Status & Status_Ground) {

		// If Knuckles has the ShovelClaw and the floor is diggable, dig.
		if (co2->Upgrades & Upgrades_ShovelClaw && (co2->SurfaceFlags & ColFlags_Dig)) {
			co2->Speed.y = 0;
			NullifyVelocity((EntityData2*)mwp, co2);
			co2->AnimationThing.Index = Anm_Knuckles_CustomDrillDig;
			data->Action = Act_Knuckles_Dig;
		}
		else {
			data->Action = Act_Knuckles_Stand;
		}
		
		return;
	}

	// Stop if colliding with solid object collision
	if (data->Status & Status_Unknown1) {
		data->Action = Act_Knuckles_Stand;
	}

	PlayerFunc_RotateToGravity(data, mwp, co2);
	RunPhysics(data, mwp, co2);

	co2->Speed.y = -KnucklesDrillSpeed;

	// All that to hurt enemies
	data->Status |= Status_Attack;
	data->CollisionInfo->CollisionArray[0].damage &= 0xFCu;
	data->CollisionInfo->CollisionArray[0].damage |= 0xCu;
	data->CollisionInfo->CollisionArray[0].damage |= 0xEF;
	data->CollisionInfo->CollisionArray[1].center = data->Position;
	data->CollisionInfo->CollisionArray[1].attr &= 0xFFFFFFEF;

	// Custom animation
	co2->AnimationThing.Index = Anm_Knuckles_CustomDrillClaw;

	if (FrameCounterUnpaused % 2 == 0) {
		LoadKnucklesAfterImages(data, co2);
	}
}

void Knuckles_SpinDash(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (Knuckles_RunNextAction(co2, mwp, data)) {
		return;
	}

	CommonSpinDash_Run(data, mwp, co2, KnucklesSpinDashMaxSpeed, KnucklesSpinDashSpeedIncrement, Anm_Knuckles_Roll, Act_Knuckles_Roll);
}

void Knuckles_TailsGrab(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	if (Knuckles_RunNextAction(co2, mwp, data)) {
		data->LoopData = nullptr;
		co2->Powerups &= ~Powerups_Invincibility;
		return;
	}

	TailsGrabAction(data, mwp, co2, { 0.0f, -8.0f, -1.0f }, Anm_Knuckles_Hang, Act_Knuckles_Fall, Anm_Knuckles_Fall);
}

void Knuckles_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	switch (data->Action) {
	case Act_Knuckles_Init:
		// Set up the fast transition dig from drill
		DrillDigAnim.Animation = KnucklesAnimData[Anm_Knuckles_DigStart].Animation;
		KnucklesAnimData[Anm_Knuckles_CustomDrillDig] = DrillDigAnim;

		// Set up the model for the custom animation
		DrillClawAnim.Animation->object = KNUCKLES_OBJECTS[0];
		KnucklesAnimData[Anm_Knuckles_CustomDrillClaw] = DrillClawAnim;
		break;
	case Act_Knuckles_Stand:
	case Act_Knuckles_Walk:
		Knuckles_CheckSpinDash(data, co2);
		break;
	case Act_Knuckles_DigOff:

		// If stopped digging from wall, go back to climb
		if (data->field_A == Act_Knuckles_Climb) {
			data->Action = Act_Knuckles_Climb;
			data->field_A = 0;
		}
		
		break;
	case Act_Knuckles_Climb:
		Knuckles_CheckWallDig(data, co2);
		break;
	case Act_Knuckles_Jump:
	case Act_Knuckles_Glide:
		Knuckles_CheckDrillClaw(data, co2);
		break;
	case Act_Knuckles_TailsGrab:
		Knuckles_TailsGrab(data, mwp, co2);
		break; 
	case Act_Knuckles_SpinDash:
		Knuckles_SpinDash(data, mwp, co2);
		break;
	case Act_Knuckles_DrillClaw:
		Knuckles_DrillClaw(data, mwp, co2);
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
	EnableKnucklesDrillClaw = config->getBool("Knuckles", "EnableKnucklesDrillClaw", true);
	EnableKnucklesWallDig = config->getBool("Knuckles", "EnableKnucklesWallDig", true);
	DiggableRMWalls = config->getBool("Knuckles", "DiggableRMWalls", true);

	KnucklesSpinDashMaxInitialSpeed = physics->getFloat("Knuckles", "SpinDashMaxInitialSpeed", 1.45f);
	KnucklesSpinDashMaxSpeed = physics->getFloat("Knuckles", "SpinDashMaxSpeed", 8.0f);
	KnucklesSpinDashSpeedIncrement = physics->getFloat("Knuckles", "SpinDashSpeedIncrement", 0.28f);
	KnucklesDrillSpeed = physics->getFloat("Knuckles", "KnucklesDrillSpeed", 7.0f);

	// Load custom Drill Claw animation
	LoadAnimation(&DrillClawMotion, "drillclaw", helperFunctions);
	DrillClawAnim.Animation = new NJS_ACTION;
	DrillClawAnim.Animation->motion = DrillClawMotion->getmotion();
}