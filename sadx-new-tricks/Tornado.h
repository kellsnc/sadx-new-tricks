#pragma once

struct tornadewk
{
	float ofsx;
	float ofsy;
	float ofsz;
	float pathx;
	float pathy;
	float pathz;
	float speed;
	float Alpha;
	int rotx;
	int roty;
	int rotz;
	int hp;
	int score;
	int RapidFire;
	int RapidCnt;
	int LockTame;
};

void Tornado_Init(const HelperFunctions& helperFunctions, const IniFile* config);
