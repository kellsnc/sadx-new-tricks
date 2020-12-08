#pragma once

void RunPhysics(EntityData1* data, motionwk* mwp, CharObj2* co2);

void TailsGrabAction(EntityData1* data, motionwk* mwp, CharObj2* co2, NJS_VECTOR offset, int animation, int fallaction, int fallanim);

void CommonSpinDash_Run(EntityData1* data, motionwk* mwp, CharObj2* co2, float maxspeed, float speedincrease, int rollanim, int rollaction);
void CommonSpinDash_Check(EntityData1* data, CharObj2* co2, int jumpspinanim, int spindashact, float maxstartspeed);