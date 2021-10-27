#pragma once

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())
#define STATUS_FLOOR (Status_Ground | Status_Unknown1)

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);

float GetLength(NJS_VECTOR* orig, NJS_VECTOR* dest);
float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);
bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius);
int IsPlayerInsideSphere_(NJS_VECTOR* center, float radius);
int IsPlayerInsideSphere_(float x, float y, float z, float radius);
bool IsSpecificPlayerInSphere(float x, float y, float z, float radius, int player);
bool IsSpecificPlayerInSphere(NJS_VECTOR* center, float radius, int player);