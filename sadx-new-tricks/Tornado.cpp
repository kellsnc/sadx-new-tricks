#include "pch.h"
#include "Tornado.h"

TaskHook Tornado_Exec_t((intptr_t)Tornado_Main);

static void Tornado_Exec_r(task* tsk)
{
	auto data = (EntityData1*)tsk->twp;

	Tornado_Exec_t.Original(tsk);

	if (data->Action > 6)
	{
		tsk->disp(tsk);
	}
}

void Tornado_Init(const HelperFunctions& helperFunctions, const IniFile* config)
{
	Tornado_Exec_t.Hook(Tornado_Exec_r);

	//WriteData<1>((char*)0x626D9C, 0x5);
}