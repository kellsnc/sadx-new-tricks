#pragma once

static constexpr int MaxPlayers = 4;

FunctionPointer(Float, GetGroundYPosition, (Float x, Float y, Float z, Rotation3* rot), 0x49E920);
FunctionPointer(Bool, IsEventPerforming, (), 0x42FB00);
FunctionPointer(int, GetClosestPlayerID, (NJS_VECTOR* pos), 0x441B70);
FunctionPointer(int, DetectDyncolCollision, (NJS_VECTOR* pos, NJS_VECTOR* output, Rotation3* rotation, ColFlags flagstoignore, float detectionradius), 0x439620);

FunctionPointer(void, PlayerFunc_Move, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x44BB60);
FunctionPointer(void, PlayerFunc_Acceleration, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x44C270);
FunctionPointer(void, PlayerFunc_AnalogToDirection, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x443F50);
FunctionPointer(void, PlayerFunc_RunDynamicCollision, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x44CDF0);
FunctionPointer(void, PlayerFunc_UpdateSpeed, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x43EE70);
FunctionPointer(void, PlayerFunc_RotateToGravity, (EntityData1* data, motionwk* mwp, CharObj2* co2), 0x443AD0);

FunctionPointer(void, PlayerDirectionToVector, (EntityData1* player, NJS_VECTOR* direction), 0x43EC90);

//signed int __usercall Amy_RunNextAction@<eax>(int co2@<ecx>, int mwp@<edi>, EntityData1 *data@<esi>)
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

ObjectFunc(SecondaryObjectPhysics, 0x49D730);
ObjectFunc(SetObjectStatusHeld, 0x46C120);
ObjectFunc(ObjectCrash, 0x5A95B0); // position is position, scale is direction, object is the model
ObjectFunc(UpdateSetDataAndDelete, 0x46C150);
ObjectFunc(DeleteGammaMissileIfNoTarget, 0x4CEFE0);

DataArray(AnimData, AmyAnimData, 0x3C54880, 102);
DataArray(AnimData, KnucklesAnimData, 0x3C532A0, 115);

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