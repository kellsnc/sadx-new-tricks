#pragma once

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())
#define STATUS_FLOOR (Status_Ground | Status_OnColli)

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);
bool CheckControl(int id);
void __cdecl PlayerClearSpeed(EntityData2* a1, CharObj2* a2);