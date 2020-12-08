#pragma once

#define Pos3(data) data[0], data[1], data[2]

void njTranslateX(float f);
void njTranslateY(float f);
void njTranslateZ(float f);
void njRotateZXY(Rotation3* rot);
void njRotateYXZ(Rotation3* rot);
void njScaleX(float f);
void njScaleY(float f);
void njScaleZ(float f);
void njScalef(float f);
void njRotateX_(Angle x);
void njRotateY_(Angle x);
void njRotateZ_(Angle x);
void njLookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);