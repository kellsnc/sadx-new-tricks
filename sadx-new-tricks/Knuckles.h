#pragma once

enum KnucklesActions : Uint8 {
	Act_Knuckles_Init,
	Act_Knuckles_Stand,
	Act_Knuckles_Walk,
	Act_Knuckles_Path,
	Act_Knuckles_Roll,
	Act_Knuckles_DashSpeedPanel,
	Act_Knuckles_Jump,
	Act_Knuckles_Spring,
	Act_Knuckles_Launch,
	Act_Knuckles_Fall,
	Act_Knuckles_Brake,
	Act_Knuckles_Glide,
	Act_Knuckles_GlideOutOfBounds,
	Act_Knuckles_Push,
	Act_Knuckles_Damage,
	Act_Knuckles_Back,
	Act_Knuckles_ObjectControl,
	Act_Knuckles_Knocked,
	Act_Knuckles_MaxHeatAttack,
	Act_Knuckles_Dig,
	Act_Knuckles_DigOut,
	Act_Knuckles_DigOff,
	Act_Knuckles_CantDig1,
	Act_Knuckles_CantDig2,
	Act_Knuckles_Climb,
	Act_Knuckles_ClimbEnd,
	Act_Knuckles_Punch,	
	Act_Knuckles_Pick,
	Act_Knuckles_Put,
	Act_Knuckles_Hold,
	Act_Knuckles_HoldTree,
	Act_Knuckles_ObjectWalk,
	Act_Knuckles_Throw,
	Act_Knuckles_ObjectBrake,
	Act_Knuckles_ObjectFall,
	Act_Knuckles_ObjectJump,
	Act_Knuckles_ObjectSpring,
	Act_Knuckles_ObjectLaunched,
	Act_Knuckles_Pull,
	Act_Knuckles_AirThrow,
	Act_Knuckles_ShakeTree,
	Act_Knuckles_Shaken,
	Act_Knuckles_MonkeyBars,
	Act_Knuckles_MonkeyBarsOn,
	Act_Knuckles_Ladder,
	Act_Knuckles_LadderOn,
	Act_Knuckles_JumpPanel,
	Act_Knuckles_JumpPanelOn,
	Act_Knuckles_Frozen,
	Act_Knuckles_Event,
	Act_Knuckles_Drown,
	Act_Knuckles_Death,
	Act_Knuckles_BumperCar,
	Act_Knuckles_PetStart,
	Act_Knuckles_Pet,
	Act_Knuckles_PetSop,
	Act_Knuckles_Whistle,
	Act_Knuckles_Debug,

	// Custom:

	Act_Knuckles_TailsGrab,
	Act_Knuckles_SpinDash,
	Act_Knuckles_DrillClaw
};

enum KnucklesAnimations {
	Anm_Knuckles_Stand,
	Anm_Knuckles_Fall = 6,
	Anm_Knuckles_JumpOrSpin = 14,
	Anm_Knuckles_Roll = 15,
	Anm_Knuckles_Roll2,
	Anm_Knuckles_Uncurl = 33,
	Anm_Knuckles_GlideCancelRoll,
	Anm_Knuckles_DigStart = 41,
	Anm_Knuckles_Dig,
	Anm_Knuckles_DigEmerge,
	Anm_Knuckles_DigJump,
	Anm_Knuckles_Hang = 84,

	// Custom:
	Anm_Knuckles_CustomDrillClaw = 57,
	Anm_Knuckles_CustomDrillDig,
	Anm_Knuckles_Drill
};