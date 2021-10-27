#pragma once

static constexpr int MaxPlayers = 8;

FunctionPointer(void, DrawObject, (NJS_OBJECT*), 0x408530);
FunctionPointer(Float, GetGroundYPosition, (Float x, Float y, Float z, Rotation3* rot), 0x49E920);
FunctionPointer(Bool, IsEventPerforming, (), 0x42FB00);
FunctionPointer(int, GetClosestPlayerID, (NJS_VECTOR* pos), 0x441B70);
FunctionPointer(int, DetectDyncolCollision, (NJS_VECTOR* pos, NJS_VECTOR* output, Rotation3* rotation, ColFlags flagstoignore, float detectionradius), 0x439620);

FunctionPointer(void, PlayerFunc_Move, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x44BB60);
FunctionPointer(void, PlayerFunc_Acceleration, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x44C270);
FunctionPointer(void, PlayerFunc_AnalogToDirection, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x443F50);
FunctionPointer(int, PlayerFunc_RunDynamicCollision, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x44CDF0);
FunctionPointer(void, PlayerFunc_UpdateSpeed, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x43EE70);
FunctionPointer(void, PlayerFunc_RotateToGravity, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x443AD0);
FunctionPointer(void, PlayerFunc_RollPhysics, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x448E50);

FunctionPointer(void, PlayerDirectionToVector, (EntityData1* player, NJS_VECTOR* direction), 0x43EC90);

ObjectFunc(SecondaryObjectPhysics, 0x49D730);
ObjectFunc(SetObjectStatusHeld, 0x46C120);
ObjectFunc(ObjectCrash, 0x5A95B0); // position is position, scale is direction, object is the model
ObjectFunc(UpdateSetDataAndDelete, 0x46C150);
ObjectFunc(DeleteGammaMissileIfNoTarget, 0x4CEFE0);

DataArray(AnimData, AmyAnimData, 0x3C54880, 102);
DataArray(AnimData, KnucklesAnimData, 0x3C532A0, 115);

//signed int __usercall Knuckles_RunNextAction@<eax>(CharObj2 *co2@<edi>, EntityData1 *data@<esi>, EntityData2 *mwp)
static const void* const Knuckles_RunNextActionPtr = (void*)0x476970;
static inline signed int Knuckles_RunNextAction(CharObj2* co2, motionwk* mwp, EntityData1* data)
{
    signed int result;
    __asm
    {
        push[mwp]
        mov esi, [data]
        mov edi, [co2]
        call Knuckles_RunNextActionPtr
        add esp, 4
        mov result, eax
    }
    return result;
}

//signed int __usercall Tails_RunNextAction@<eax>(CharObj2 *co2@<edi>, EntityData1 *data@<esi>, EntityData2 *mwp)
static const void* const Tails_RunNextActionPtr = (void*)0x45C100;
static inline signed int Tails_RunNextAction(CharObj2* co2, motionwk* mwp, EntityData1* data)
{
    signed int result;
    __asm
    {
        push[mwp]
        mov esi, [data]
        mov edi, [co2]
        call Tails_RunNextActionPtr
        add esp, 4
        mov result, eax
    }
    return result;
}

//signed int __usercall Amy_RunNextAction@<eax>(CharObj2* co2@<ecx>, motionwk* mwp@<edi>, EntityData1 *data@<esi>)
static const void* const Amy_RunNextActionPtr = (void*)0x487810;
static inline signed int Amy_RunNextAction(CharObj2* co2, motionwk* mwp, EntityData1* data)
{
    signed int result;
    __asm
    {
        mov ecx, [co2]
        mov edi, [mwp]
        mov esi, [data]
        call Amy_RunNextActionPtr
        mov result, eax
    }
    return result;
}

//void __usercall PutPlayerBehind(NJS_VECTOR* pos@<edi>, EntityData1* data@<esi>, float dist)
static const void* const PutPlayerBehindPtr = (void*)0x47DD50;
static inline void PutPlayerBehind(NJS_VECTOR* pos, EntityData1* data, float dist)
{
    __asm
    {
        push[dist]
        mov esi, [data]
        mov edi, [pos]
        call PutPlayerBehindPtr
        add esp, 4
    }
}

static inline void SetObjectStatusNotHeld(ObjectMaster* obj) {
    obj->SETData.SETData->Flags &= ~0x70;
}

enum CollisionShapes {
    CollisionShape_Sphere,
    CollisionShape_Cylinder,
    CollisionShape_PushCylinder,
    CollisionShape_Cube,
    CollisionShape_Cube2,
    CollisionShape_Capsule,
    CollisionShape_Capsule2
};