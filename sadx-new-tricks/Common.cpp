#include "pch.h"

void RunPhysics(EntityData1* data, motionwk* mwp, CharObj2* co2) {
	PlayerFunc_Move(data, mwp, co2);
	PlayerFunc_Acceleration(data, mwp, co2);
	PlayerFunc_AnalogToDirection(data, mwp, co2);
	PlayerFunc_RunDynamicCollision(data, mwp, co2);
	PlayerFunc_UpdateSpeed(data, mwp, co2);
}

#pragma region TailsGrab
void TailsGrabAction_Stop(EntityData1* data, CharObj2* co2, int endaction, int fallanim) {
	data->Action = endaction;
	co2->AnimationThing.Index = fallanim;
	data->LoopData = nullptr;
	co2->Powerups &= ~Powerups_Invincibility;
}

void TailsGrabAction_Run(EntityData1* grabplayer, EntityData1* data, motionwk* mwp, CharObj2* co2, NJS_VECTOR offset, int animation, int fallaction, int fallanim) {
	// Release if grabbed player touches the ground
	if (data->Status & Status_Ground) {
		data->Action = 1;
		data->LoopData = nullptr;
		PlaySound(33, 0, 0, 0);
		co2->Powerups &= ~Powerups_Invincibility;
		PutPlayerBehind(&data->Position, grabplayer, 15.0f);
		return;
	}

	// Release if grabbing player not holding anymore
	if (grabplayer->field_A == 0) {
		TailsGrabAction_Stop(data, co2, fallaction, fallanim);
		return;
	}

	// Release if jumping
	if (PressedButtons[data->CharIndex] & JumpButtons) {
		TailsGrabAction_Stop(data, co2, fallaction, fallanim);
		return;
	}
	
	// Some adjustements
	co2->Powerups |= Powerups_Invincibility;
	co2->Speed = { 0, 0, 0 };
	NullifyVelocity((EntityData2*)mwp, co2);

	// Move the player to the grabbing player
	data->Position = grabplayer->Position;
	PlayerDirectionToVector(grabplayer, &offset);
	njAddVector(&data->Position, &offset);

	// Rotate
	data->Rotation = grabplayer->Rotation;

	// Set the hook animation
	co2->AnimationThing.Index = animation;

	// Run ground check
	PlayerFunc_RunDynamicCollision(data, mwp, co2);
}

void TailsGrabAction(EntityData1* data, motionwk* mwp, CharObj2* co2, NJS_VECTOR offset, int animation, int fallaction, int fallanim) {
	EntityData1* grabplayer = (EntityData1*)data->LoopData;

	// Be sure it's a player, otherwise stop the grab action
	if (grabplayer) {
		for (int i = 0; i < MaxPlayers; ++i) {
			if (EntityData1Ptrs[i] == grabplayer) {
				TailsGrabAction_Run(grabplayer, data, mwp, co2, offset, animation, fallaction, fallanim);
				return;
			}
		}
	}
	
	TailsGrabAction_Stop(data, co2, fallaction, fallanim);
}
#pragma endregion

#pragma region SpinDash

void CommonSpinDash_Run(EntityData1* data, motionwk* mwp, CharObj2* co2, float maxspeed, float speedincrease, int rollanim, int rollaction) {
	if (co2->Speed.x > 0) {
		co2->Speed.x -= 0.1f;
	}

	++co2->SonicSpinTimer;

	if (co2->SonicSpinTimer < 300) {
		if (HeldButtons[data->CharIndex] & Buttons_B) {
			if (co2->SpindashSpeed < maxspeed) {
				co2->SpindashSpeed += speedincrease;
			}
		}
		else {
			DoSoundQueueThing(767);
			DoSoundQueueThing(763);
			data->Action = rollaction;
			co2->AnimationThing.Index = rollanim;
			co2->Speed.x = co2->SpindashSpeed;
			co2->SpindashSpeed = 0;
			co2->SonicSpinTimer = 0;
		}
	}
	else {
		DoSoundQueueThing(767);
		DoSoundQueueThing(763);
		data->Action = 1;
		co2->IdleTime = 0;
		data->Status &= ~(Status_Ball | Status_Attack);
	}

	RunPhysics(data, mwp, co2);
}

void CommonSpinDash_Check(EntityData1* data, CharObj2* co2, int jumpspinanim, int spindashact, float maxstartspeed) {
	if (PressedButtons[data->CharIndex] & Buttons_B) {
		co2->AnimationThing.Index = jumpspinanim;
		data->Status |= Status_Attack | Status_Ball;
		data->Action = spindashact;
		co2->SpindashSpeed = fmin(maxstartspeed, co2->Speed.x);
		QueueSound_DualEntity(767, data, 1, 0, 2);
		co2->SonicSpinTimer = 0;
	}
}

#pragma endregion