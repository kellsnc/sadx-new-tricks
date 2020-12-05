#pragma once

enum SonicActions : Uint8 {
	Act_Sonic_Init,
	Act_Sonic_Stand,
	Act_Sonic_Walk,
	Act_Sonic_Path,
	Act_Sonic_Spindash,
	Act_Sonic_Roll,
	Act_Sonic_LightSpeedDash,
	Act_Sonic_DashSpeedPanel,
	Act_Sonic_Jump,
	Act_Sonic_Spring,
	Act_Sonic_Launch,
	Act_Sonic_Paralyzed,
	Act_Sonic_Fall,
	Act_Sonic_Brake,
	Act_Sonic_HomingAttack,
	Act_Sonic_Push,
	Act_Sonic_Damage,
	Act_Sonic_Back,
	Act_Sonic_Knocked,
	Act_Sonic_Slide,
	Act_Sonic_ObjectControl,
	Act_Sonic_Pick,
	Act_Sonic_Put,
	Act_Sonic_Hold,
	Act_Sonic_HoldTree,
	Act_Sonic_ObjectWalk,
	Act_Sonic_Throw,
	Act_Sonic_ObjectBrake,
	Act_Sonic_ObjectFall,
	Act_Sonic_ObjectJump,
	Act_Sonic_ObjectSpring,
	Act_Sonic_ObjectLaunched,
	Act_Sonic_Pull,
	Act_Sonic_AirThrow,
	Act_Sonic_ShakeTree,
	Act_Sonic_Shaken,
	Act_Sonic_MonkeyBars,
	Act_Sonic_MonkeyBarsOn,
	Act_Sonic_Ladder,
	Act_Sonic_LadderOn,
	Act_Sonic_JumpPanel,
	Act_Sonic_JumpPanelOn,
	Act_Sonic_Captured,
	Act_Sonic_Frozen,
	Act_Sonic_LightSpeedAttack,
	Act_Sonic_SHBuildingRun,
	Act_Sonic_SHBuildingRoll,
	Act_Sonic_SHBuildingFall,
	Act_Sonic_SHBuildingJump,
	Act_Sonic_SHBuildingHurt,
	Act_Sonic_BumperCar,
	Act_Sonic_SkyDeckCyl,
	Act_Sonic_SkyDeckCylDown,
	Act_Sonic_SkyDeckCylLeft,
	Act_Sonic_SkyDeckCylRight,
	Act_Sonic_IcicleHang,
	Act_Sonic_IcicleSlip,
	Act_Sonic_IcicleRotL,
	Act_Sonic_IcicleRotR,
	Act_Sonic_IcicleJump,
	Act_Sonic_IcicleClimb,
	Act_Sonic_IcicleLaunch,
	Act_Sonic_BoardSlide,
	Act_Sonic_BoardSlideF,
	Act_Sonic_BoardBrake,
	Act_Sonic_BoardFall,
	Act_Sonic_BoardJump,
	Act_Sonic_BoardTrick,
	Act_Sonic_BoardHurt,
	Act_Sonic_PinBall,
	Act_Sonic_PinBallFall,
	Act_Sonic_PetStart,
	Act_Sonic_Pet,
	Act_Sonic_PetSop,
	Act_Sonic_Whistle,
	Act_SuperSonic_Stand,
	Act_SuperSonic_Walk,
	Act_SuperSonic_Path,
	Act_SuperSonic_Fall,
	Act_SuperSonic_Launched,
	Act_SuperSonic_Spring,
	Act_SuperSonic_Homing,
	Act_SuperSonic_Jump,
	Act_Sonic_Shower,
	Act_Sonic_Event,
	Act_Sonic_Drown,
	Act_Sonic_Death,
	Act_Sonic_Debug,

	// Custom:

	Act_Sonic_TailsGrab
};

enum SonicAnimations {
	Anm_Sonic_Stand,
	Anm_Sonic_Fall = 18,
	Anm_Sonic_Hang = 47
};