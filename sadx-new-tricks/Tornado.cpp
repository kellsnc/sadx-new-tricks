#include "pch.h"
#include "Tornado.h"

DataPointer(tornadewk, Tornade, 0x3C822A8);

static Trampoline* Tornado_Exec_t = nullptr;

static void Tornado_NewActions(EntityData1* data, motionwk* mwp, CharObj2* co2)
{
	switch (data->Action)
	{
	case 1: // Normal
		
		break;
	}
}

static void Tornado_Exec_r(task* tsk)
{
	EntityData1* data = (EntityData1*)tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing movement information
	CharObj2* co2 = (CharObj2*)mwp->work.ptr; // physics, animation info, and countless other things

	Tornado_NewActions(data, mwp, co2);
	
	TRAMPOLINE(Tornado_Exec)(tsk);

	if (data->Action > 6)
	{
		tsk->disp(tsk);
	}
}

void Tornado_Init(const HelperFunctions& helperFunctions, const IniFile* config)
{
	Tornado_Exec_t = new Trampoline((int)Tornado_Main, (int)Tornado_Main + 0x5, Tornado_Exec_r);

	//WriteData<1>((char*)0x626D9C, 0x5);
}