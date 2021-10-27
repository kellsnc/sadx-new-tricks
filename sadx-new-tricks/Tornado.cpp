#include "pch.h"
#include "Tornado.h"

DataPointer(tornadewk, Tornade, 0x3C822A8);

static Trampoline* Tornado_Exec_t = nullptr;

static void Tornado_Exec_r(task* tsk)
{
	auto data = (EntityData1*)tsk->twp;

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