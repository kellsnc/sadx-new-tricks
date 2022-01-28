#pragma once

// SADX definitions thanks to ItsEasyActually



typedef void(__cdecl* CameraFuncPtr)(_OBJ_CAMERAPARAM*);



#define CameraAdjustFunc(NAME, ADDRESS) FunctionPointer(void, NAME, (EntityData1* current_camdata, EntityData1* previous_camdata, _OBJ_ADJUSTPARAM* pParam), ADDRESS)
#define CameraFunc(NAME, ADDRESS) FunctionPointer(void, NAME, (_OBJ_CAMERAPARAM *pParam), ADDRESS)

CameraAdjustFunc(CameraAdjust_None, 0x467D80);
CameraAdjustFunc(CameraAdjust_Normal, 0x467DC0);
CameraAdjustFunc(CameraAdjust_ForFreeCam, 0x468800);

CameraFunc(Camera_Knuckles, 0x469590);

enum class CameraModesIDs : __int16 {
	Follow,
	A_Follow,
	C_Follow,
	COL_Follow,
	Knuckles,
	A_Knuckles,
	C_Knuckles,
	COL_Knuckles,
	Knuckles2,
	A_Knuckles2,
	C_Knuckles2,
	COL_Knuckles2,
	Magonote,
	A_Magonote,
	C_Magonote,
	COL_Magonote,
	Sonic,
	A_Sonic,
	C_Sonic,
	COL_Sonic,
	Ashland,
	A_Ashland,
	C_Ashland,
	Ashland_I,
	A_Ashland_I,
	C_Ashland_I,
	Fishing,
	A_Fishing,
	Fixed,
	A_Fixed,
	C_Fixed,
	KlaMath,
	A_KlaMath,
	C_KlaMath,
	Line,
	A_Line,
	NewFollow,
	A_NewFollow,
	C_NewFollow,
	Point,
	A_Point,
	C_Point,
	Sonic_P,
	A_Sonic_P,
	C_Sonic_P,
	Advertise,
	Back,
	Back2,
	Building,
	Cart,
	Chaos,
	Chaos_P,
	Chaos_STINIT,
	Chaos_STD,
	Chaos_W,
	E101R,
	E103,
	EGM3,
	Follow_G,
	A_Follow_G,
	LR,
	COLLI,
	RuinWaka1,
	SnowBoard,
	Survey,
	TAIHO,
	Tornade,
	Two_Hares,
	Leave,
	Avoid,
	A_Avoid,
	C_Avoid,
	Col_Avoid,
	Editor,
	GuriGuri,
	PathCam
};

enum class CameraAdjustsIDs : __int8 {
	None,
	Normal,
	Normal_S,
	Slow,
	Slow_S,
	Time,
	Three1,
	Three1C,
	Three2,
	Three2C,
	Three3,
	Three3C,
	Three4,
	Three4C,
	Three5,
	Three5C,
	Relative1,
	Relative1C,
	Relative2,
	Relative2C,
	Relative3,
	Relative3C,
	Relative4,
	Relative4C,
	Relative5,
	Relative5C,
	Relative6,
	FreeCam
};

enum class CameraDirectIDs : __int8 {
	Sonic,
	Unknown,
	Target
};

// The list of camera modes; this list is used by camera files; enum : CameraModesIDs
DataArray(_OBJ_CAMERAMODE, CameraModes, 0x975410, 77);

// The list of camera adjusts; this list is used by camera files; enum : CameraAdjustsIDs
DataArray(_OBJ_CAMERAADJUST, CameraAdjusts, 0x975410, 28);

// Pointer to the camera system structure, contains the current auto cameras informations
DataPointer(_CameraSystemWork, CameraSystem, 0x3B2CAD8);

// The camera information, contains its position, rotation, etc.
DataPointer(_camcontwk, CameraTask, 0x3B2C660);

FunctionPointer(void, CameraSetNormalCamera, (CameraModesIDs ssCameraMode, CameraAdjustsIDs ucAdjustType), 0x436040);
FunctionPointer(void, SetCameraEvent, (CameraFuncPtr func, CameraAdjustsIDs adjust, CameraDirectIDs direct), 0x437D20);
FunctionPointer(void, CameraBackInit, (int mode), 0x629680);

VoidFunc(RemoveCameraEvent, 0x436140);
