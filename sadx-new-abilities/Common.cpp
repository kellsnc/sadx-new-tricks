#include "pch.h"

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
		PutPlayerBehind(&data->Position, grabplayer, 15.0f);
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