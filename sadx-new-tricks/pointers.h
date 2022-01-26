#pragma once

static constexpr int MaxPlayers = 8;

FunctionPointer(Float, GetGroundYPosition, (Float x, Float y, Float z, Rotation3* rot), 0x49E920);
FunctionPointer(Bool, IsEventPerforming, (), 0x42FB00);
FunctionPointer(int, GetClosestPlayerID, (NJS_VECTOR* pos), 0x441B70);
FunctionPointer(int, DetectDyncolCollision, (NJS_VECTOR* pos, NJS_VECTOR* output, Rotation3* rotation, ColFlags flagstoignore, float detectionradius), 0x439620);

FunctionPointer(void, PlayerUpdateSpeed, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x43EE70);
FunctionPointer(void, PlayerRollPhysics, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x448E50);
FunctionPointer(void, PGlideAcceleration, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x448000);
FunctionPointer(void, PGlideGetSpeed, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x444580);

ObjectFunc(SecondaryObjectPhysics, 0x49D730);
ObjectFunc(SetObjectStatusHeld, 0x46C120);
ObjectFunc(ObjectCrash, 0x5A95B0); // position is position, scale is direction, object is the model
ObjectFunc(UpdateSetDataAndDelete, 0x46C150);
ObjectFunc(DeleteGammaMissileIfNoTarget, 0x4CEFE0);

DataArray(AnimData, AmyAnimData, 0x3C54880, 102);
DataArray(AnimData, KnucklesAnimData, 0x3C532A0, 115);

//signed int __usercall Knuckles_RunNextAction@<eax>(CharObj2 *co2@<edi>, EntityData1 *data@<esi>, EntityData2 *mwp)
static const void* const Knuckles_RunNextActionPtr = (void*)0x476970;
static inline signed int Knuckles_RunNextAction(CharObj2* co2, motionwk2* mwp, EntityData1* data)
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
static inline signed int Tails_RunNextAction(CharObj2* co2, motionwk2* mwp, EntityData1* data)
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

//signed int __usercall Amy_RunNextAction@<eax>(CharObj2* co2@<ecx>, motionwk2* mwp@<edi>, EntityData1 *data@<esi>)
static const void* const Amy_RunNextActionPtr = (void*)0x487810;
static inline signed int Amy_RunNextAction(CharObj2* co2, motionwk2* mwp, EntityData1* data)
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

//signed int __usercall PGetColliInfo@<eax>(EntityData1* a1@<eax>, EntityData2* a2, CharObj2* a3)
static const void* const PGetColliInfoPtr = (void*)0x4746C0;
static inline signed int PGetColliInfo(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
    signed int result;
    __asm
    {
        push[pwp]
        push[mwp]
        mov eax, [twp]
        call PGetColliInfoPtr
        mov result, eax
        add esp, 8
    }
    return result;
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

FunctionPointer(bool, CheckCollisionForPanelJump, (EntityData1* data), 0x4B83C0);
FunctionPointer(signed int, CanIMakeJumpPanel, (EntityData1* data), 0x4B83F0);
FunctionPointer(void, StartPlayerPanelJump, (EntityData1* data), 0x4B8470);
