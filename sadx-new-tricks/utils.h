#pragma once

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())
#define STATUS_FLOOR (Status_Ground | Status_OnColli)
#define TASKWK_PLAYERID(a) a->counter.b[0]
#define TASKWK_CHARID(a) a->counter.b[1]

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);
bool CheckControl(int id);
void PlayerClearSpeed(motionwk2* mwp, playerwk* pwp);
bool isSuper(playerwk* pwp);
bool isSuper(int pnum);
bool isTailsAI(int pnum);