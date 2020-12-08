# SADX New Tricks
New abilities and upgrades for SADX characters.
[Download here](https://gamebanana.com/gamefiles/7104).

This mod creates *actual new actions* (or "modes" internally) to character, so every character objects are hooked. It means you can for example force one of the custom actions below from another mod.

*Individual character abilities can be disabled* using the ModLoader's configuration form. You can also *disable character hooks individually* to disable every custom action for the given character. Disabling a character hook also means he will no longer be able to be grabbed by Tails.

## New Actions
### Sonic
* Can be grabbed by Tails.

### Tails
* **TailsGrab**: ability to grab characters while flying. *(Enabled by default.)*
    * Ability to grab Sonic, Metal Sonic, Tails, Knuckles and Amy if they jump onto flying Tails. 
    * If Tails or the grabbed character touch the ground or if you press B, the character will be dropped.
* **SpinDash**: you can use the SpinDash like Sonic by pressing B on the ground. *(Enabled by default.)*
* **SpinTails**: Tails' tails will be visible when rolling/spindashing. *(Disabled by default, experimental.)*
* Can be grabbed by Tails.

### Knuckles
* **DrillClaw**: Knuckles can drill down by pressing the B button while jumping/glinding.  *(Enabled by default.)*
* **WallDig**: allow Knuckles to dig in walls. SADX walls do not have the "diggable" flag on wall collision however, but there is an option in the configuration to remove that condition.  *(Enabled by default.)*
* **SpinDash**: you can use the SpinDash like Sonic by pressing B on the ground. *(Enabled by default.)*
* Can be grabbed by Tails.

### Amy
* **Hammer Propeller**: Amy can propel herself with her hammer by holding the action button while in air. *(Enabled by default.)*
* **Double Jump**: Amy can double jump by pressind the jump button a second time. The longer you wait, the less the double jump impact will be.  *(Enabled by default.)*
* Can be grabbed by Tails.

## Configuration
Use the modloader's configuration form, or create a config.ini using the configs below. <br>
Default value is between brackets, no need to put it in the config.ini if you're not changing the value.

Example:
```
[Tails]
EnableTailsGrab=False
EnableTailsSpinTails=True
```

* General: these will disable hooks for individual characters, effectively disabling every custom action for that character.
    * *EnableSonic* (true)
    * *EnableTails* (true)
    * *EnableKnuckles* (true)
    * *EnableAmy* (true)
    * *EnableGamma* (true) (for later)
    * *EnableBig* (true) (for later)
    * *EnableTornado* (true) (for later)
* Tails
    * *EnableTailsGrab*: enable/disable (true)
    * *EnableTailsSpinDash*: enable/disable (true)
    * *EnableTailsSpinTails*: enable/disable tails' tails out of his spinning ball (false)
* Knuckles
    * *EnableKnucklesSpinDash*: enable/disable (true)
    * *EnableKnucklesDrillClaw*: enable/disable (true)
    * *EnableKnucklesWallDig*: enable/disable (true)
    * *DiggableRMWalls*: Make Red Mountain walls diggable (true)
* Amy
    * *EnableDoubleJump*: enable/disable (true)
    * *EnableHammerPropeller*: enable/disable (true)
 
### New Physics
Located in physics.ini, you can tweak them if you want to but it's not recommended. Note that it will be overwritten by updates.

* Tails
    * *SpinDashMaxInitialSpeed*: speed if you release the spindash immidietally (1.25)
    * *SpinDashMaxSpeed*: max spindash speed you can charge (7.0)
    * *SpinDashSpeedIncrement*: the charge speed increment per frame (0.2)
* Knuckle
    * *SpinDashMaxInitialSpeed*: speed if you release the spindash immidietally (1.45)
    * *SpinDashMaxSpeed*: max spindash speed you can charge (8.0)
    * *SpinDashSpeedIncrement*: the charge speed increment per frame (0.28)
    * *KnucklesDrillSpeed*: the speed at which Knuckles will descend (7.0)
* Amy
    * *PropellerGravity*: the gravity alteration when in propeller mode, keep it low (0.011)
    * *PropellerInitialAccTreshold*: max initial acceleration speed (1.0)
    * *PropellerInitialAcc*: speed acceleration if the input speed of the character is below *PropellerInitialAccTreshold* (1.01)
    * *PropellerAirAccTreshold*: max acceleration speed (7.0)
    * *PropellerAirAcc*: speed acceleration if the input speed of the character is below *PropellerAirAccTreshold* (1.005)
    * *DoubleJumpAcc*: vertical speed added by the double jump (1.12)
