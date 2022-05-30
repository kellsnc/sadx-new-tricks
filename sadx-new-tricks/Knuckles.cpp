#include "pch.h"

static bool EnableKnucklesSpinDash  = true;
static bool EnableKnucklesDrillClaw = true;
static bool EnableKnucklesWallDig   = true;
static bool DiggableRMWalls         = true;

static float KnucklesSpinDashMaxInitialSpeed = 1.45f;
static float KnucklesSpinDashMaxSpeed        = 8.0f;
static float KnucklesSpinDashSpeedIncrement  = 0.28f;
static float KnucklesDrillSpeed              = 7.0f;

static Trampoline* Knuckles_Exec_t       = nullptr;
static Trampoline* Knuckles_RunActions_t = nullptr;

static PL_ACTION DrillClawAnim = { nullptr, 78, 3, Anm_Knuckles_CustomDrillClaw, 0.5f, 2.0f };
static PL_ACTION DrillDigAnim  = { nullptr, 78, 4, Anm_Knuckles_Dig, 1.0f, 1.5f };

static AnimationFile* DrillClawMotion = nullptr;

static void Knuckles_AfterImages(task* tsk)
{
	if (!MissedFrames)
	{
		auto wk = tsk->twp;
		auto co2 = CharObj2Ptrs[wk->value.b[0]];

		if (IsGamePaused() == false)
		{
			wk->scl.x -= 0.1f;

			if (wk->scl.x <= 0.0f)
			{
				FreeTask(tsk);
				return;
			}
		}

		if (wk->scl.x < 0.9f)
		{
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
			njAction_Queue(DrillClawAnim.actptr, wk->scl.y, (QueuedModelFlagsB)0);
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

static void LoadKnucklesAfterImages(EntityData1* data, CharObj2* co2)
{
	// Hyper Knuckles compatibility
	if (!isSuper(data->CharIndex)) {
		auto tsk = CreateElementalTask(LoadObj_Data1, tasklevel::LEV_4, Knuckles_AfterImages);
		tsk->disp = Knuckles_AfterImages;

		auto wk = tsk->twp;

		wk->value.b[0] = data->CharIndex;
		wk->scl.x = 1.0f;
		wk->scl.y = co2->AnimationThing.Frame;
		wk->pos = data->Position;
		wk->ang = *(Angle3*)&data->Rotation;
	}
}

static void Knuckles_CheckSpinDash(EntityData1* data, CharObj2* co2)
{
	if (EnableKnucklesSpinDash == true)
	{
		CommonSpinDash_Check(data, co2, Anm_Knuckles_JumpOrSpin, Act_Knuckles_SpinDash, KnucklesSpinDashMaxInitialSpeed);
	}
}

static void Knuckles_CheckDrillClaw(EntityData1* data, CharObj2* co2)
{
	if (EnableKnucklesDrillClaw == true && CheckControl(data->CharIndex) && HeldButtons[data->CharIndex] & Buttons_B)
	{
		data->Action = Act_Knuckles_DrillClaw;
		data->Status &= ~Status_Ball;
		LoadKnucklesAfterImages(data, co2);
	}
}

static void Knuckles_CheckWallDig(EntityData1* data, CharObj2* co2)
{
	if (co2->Upgrades & Upgrades_ShovelClaw && PressedButtons[data->CharIndex] & AttackButtons && ((CurrentLevel == LevelIDs_RedMountain && CurrentAct == 2 && DiggableRMWalls) || (co2->SurfaceFlags & ColFlags_Dig)))
	{
		data->Action = Act_Knuckles_Dig;
		data->field_A = Act_Knuckles_Climb; // store next action after walldig in this
		co2->AnimationThing.Index = Anm_Knuckles_CustomDrillDig;
		co2->AnimationThing.Frame = 0.2f;
	}
}

static void Knuckles_DrillClaw(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	if (Knuckles_RunNextAction(co2, mwp, data))
	{
		return;
	}

	// Stop if B is not held anymore
	if ((HeldButtons[data->CharIndex] & Buttons_B) != Buttons_B)
	{
		data->Status |= Status_Ball;
		data->Action = Act_Knuckles_Jump;
		co2->AnimationThing.Index = Anm_Knuckles_GlideCancelRoll;
		return;
	}

	// Stop if touches the ground
	if (data->Status & STATUS_FLOOR)
	{
		// If Knuckles has the ShovelClaw and the floor is diggable, dig.
		if (co2->Upgrades & Upgrades_ShovelClaw && (co2->SurfaceFlags & ColFlags_Dig))
		{
			co2->Speed.y = 0;
			NullifyVelocity((EntityData2*)mwp, co2);
			co2->AnimationThing.Index = Anm_Knuckles_CustomDrillDig;
			data->Action = Act_Knuckles_Dig;
		}
		else
		{
			data->Action = Act_Knuckles_Stand;
		}

		return;
	}

	// Stop if colliding with solid object collision
	if (data->Status & Status_OnColli)
	{
		data->Action = Act_Knuckles_Stand;
	}

	PlayerResetAngle((taskwk*)data, mwp, (playerwk*)co2);
	RunPhysics(data, mwp, co2);

	// Hyper Knuckles compatibility
	if (!isSuper(data->CharIndex))
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

	if (FrameCounterUnpaused % 2 == 0)
	{
		LoadKnucklesAfterImages(data, co2);
	}
}

static void Knuckles_SpinDash(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	if (Knuckles_RunNextAction(co2, mwp, data))
	{
		return;
	}

	CommonSpinDash_Run(data, mwp, co2, KnucklesSpinDashMaxSpeed, KnucklesSpinDashSpeedIncrement, Anm_Knuckles_Roll, Anm_Knuckles_Uncurl, Act_Knuckles_Roll);
}

static void Knuckles_TailsGrab(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	if (Knuckles_RunNextAction(co2, mwp, data))
	{
		data->LoopData = nullptr;

		// Hyper Knuckles compatibility
		if (!isSuper(data->CharIndex))
			co2->Powerups &= ~Powerups_Invincibility;
		return;
	}

	TailsGrabAction(data, mwp, co2, { 0.0f, -8.0f, -1.0f }, Anm_Knuckles_Hang, Act_Knuckles_Fall, Anm_Knuckles_Fall);
}

static void Knuckles_RunActions_r(EntityData1* data, motionwk2* mwp, CharObj2* co2)
{
	switch (data->Action)
	{
	case Act_Knuckles_JumpPanel:
		if (KnucklesCheckInput((taskwk*)data, mwp, (playerwk*)co2))
		{
			return;
		}
		if (CheckCollisionForPanelJump(data))
		{
			PlaySound(33, 0, 0, 0);
			PlayerClearSpeed(mwp, (playerwk*)co2);
			data->Action = Act_Knuckles_JumpPanelOn;
			co2->AnimationThing.Index = 106;
		}
		else if ((data->Status & (Status_OnColli | Status_Ground)) != 0)
		{
			PlaySound(33, 0, 0, 0);
			PlayerClearSpeed(mwp, (playerwk*)co2);
			data->Action = 1;
			co2->IdleTime = 0;
			co2->AnimationThing.Index = 2;
		}
		return;
	case Act_Knuckles_JumpPanelOn:
		if (KnucklesCheckInput((taskwk*)data, mwp, (playerwk*)co2))
		{
			return;
		}
		if (CanIMakeJumpPanel(data) <= 0)
		{
			data->Action = 1;
			co2->IdleTime = 0;
			co2->AnimationThing.Index = 2;
			return;
		}
		if (JumpAllowed(data) != 2)
		{
			return;
		}
		StartPlayerPanelJump(data);
		data->Action = Act_Knuckles_JumpPanel;
		co2->AnimationThing.Index = 106;
		PlaySound(17, 0, 0, 0);
		return;
	}

	TRAMPOLINE(Knuckles_RunActions)(data, mwp, co2);
}

static void Knuckles_Exec_r(task* tsk)
{
	auto data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	auto mwp = (motionwk2*)tsk->mwp; // task containing movement information
	auto co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	switch (data->Action)
	{
	case Act_Knuckles_Init:
		// Set up the fast transition dig from drill:
		DrillDigAnim.actptr = KnucklesAnimData[Anm_Knuckles_DigStart].actptr;
		KnucklesAnimData[Anm_Knuckles_CustomDrillDig] = DrillDigAnim;

		// Set up the model for the custom animation:
		DrillClawAnim.actptr->object = KNUCKLES_OBJECTS[0];
		KnucklesAnimData[Anm_Knuckles_CustomDrillClaw] = DrillClawAnim;
		break;
	case Act_Knuckles_Stand:
	case Act_Knuckles_Walk:
		Knuckles_CheckSpinDash(data, co2);
		break;
	case Act_Knuckles_DigOff:
		if (data->field_A == Act_Knuckles_Climb) // If stopped digging from wall, go back to climb
		{
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

	// Hack to get the ball to bend during spindash
	// They made the code in the wrong order in Knuckles Display so I have to patch the condition.
	if (co2)
	{
		if (co2->AnimationThing.Index == Anm_Knuckles_JumpOrSpin)
		{
			int timer = LOWORD(co2->field_84);

			++timer;

			bool test;

			if (co2->AnimationThing.Index == 34)
			{
				test = (LOBYTE(timer) & 1) == 0;
			}
			else
			{
				test = (LOBYTE(timer) & 0x11) == 0;
			}

			if (!test)
			{
				WriteData<1>((void*)0x4723D9, Anm_Knuckles_Roll2);
			}
			else
			{
				WriteData<1>((void*)0x4723D9, Anm_Knuckles_JumpOrSpin);
			}
		}
		else if (data->Status & Status_Ball) // Prevent distorting when not spindashing
		{
			WriteData<1>((void*)0x4723D9, Anm_Knuckles_JumpOrSpin);
		}
	}

	TRAMPOLINE(Knuckles_Exec)(tsk);
}

void Knuckles_Init(const HelperFunctions& helperFunctions, const IniFile* config, const IniFile* physics)
{
	Knuckles_Exec_t = new Trampoline((int)Knuckles_Main, (int)Knuckles_Main + 0x7, Knuckles_Exec_r);
	Knuckles_RunActions_t = new Trampoline(0x478020, 0x478025, Knuckles_RunActions_r);

	auto configgrp = config->getGroup("Knuckles");

	if (configgrp)
	{
		EnableKnucklesSpinDash = configgrp->getBool("EnableKnucklesSpinDash", EnableKnucklesSpinDash);
		EnableKnucklesDrillClaw = configgrp->getBool("EnableKnucklesDrillClaw", EnableKnucklesDrillClaw);
		EnableKnucklesWallDig = configgrp->getBool("EnableKnucklesWallDig", EnableKnucklesWallDig);
		DiggableRMWalls = configgrp->getBool("DiggableRMWalls", DiggableRMWalls);
	}

	auto physgrp = physics->getGroup("Knuckles");

	if (physgrp)
	{
		KnucklesSpinDashMaxInitialSpeed = physgrp->getFloat("SpinDashMaxInitialSpeed", KnucklesSpinDashMaxInitialSpeed);
		KnucklesSpinDashMaxSpeed = physgrp->getFloat("SpinDashMaxSpeed", KnucklesSpinDashMaxSpeed);
		KnucklesSpinDashSpeedIncrement = physgrp->getFloat("SpinDashSpeedIncrement", KnucklesSpinDashSpeedIncrement);
		KnucklesDrillSpeed = physgrp->getFloat("DrillSpeed", KnucklesDrillSpeed);
	}

	// Load custom Drill Claw animation
	LoadAnimation(&DrillClawMotion, "drillclaw", helperFunctions);
	DrillClawAnim.actptr = new NJS_ACTION;
	DrillClawAnim.actptr->motion = DrillClawMotion->getmotion();

	JumpPanel_Collision_[3].center.y = 2.0f;
	JumpPanel_Collision_[3].a = 7.0f;
}