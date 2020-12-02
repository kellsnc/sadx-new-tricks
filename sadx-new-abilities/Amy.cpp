#include "pch.h"
#include "Amy.h"

Trampoline* Amy_Exec_t = nullptr;

void Amy_Exec_r(task* tsk) {
	taskwk* twp = tsk->twp; // main task containing position, rotation, scale
	motionwk* mwp = tsk->mwp; // task containing physics, animation info, and countless other things

	NonStaticFunctionPointer(void, Amy_Original, (task * tsk), Amy_Exec_t->Target());
	Amy_Original(tsk);
}

void __cdecl Amy_Init(const HelperFunctions& helperFunctions) {
	Amy_Exec_t = new Trampoline((int)Amy_Main, (int)Amy_Main + 0x8, Amy_Exec_r);
}