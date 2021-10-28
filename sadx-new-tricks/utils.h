#pragma once

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())
#define STATUS_FLOOR (Status_Ground | Status_Unknown1)

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);