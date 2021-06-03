// --------------------------------------------------------------------------------------------
// Filename: Launch-Orbit.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is to test the Starship Horizontal flight.
// 02/Jun/2021
// --------------------------------------------------------------------------------------------

// Init Common:
// --------------------------------------------------------------------------------------------
LOG   "START: starship_lowentry_return.c" to log.txt.
CLEARSCREEN.
set STAGE_1_TYPE to "CORE".

//runpath("boot/useful-tools.c").
runpath("boot/spacex_defaults.c").
runpath("boot/common.c").

DELETEPATH("1:/STAGE1_TARGET_FILE.c"). 	//KOS local HD
DELETEPATH("FLIP.txt"). 				//Real HD
DELETEPATH(STAGE_1_TYPE+"burn.txt").
	
if KUniverse:ActiveVessel <> SHIP
	update_phase_title("(WAIT TO BE ACTIVE)", 0, true).

UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.

if vehicle_type = "SN9-Profile1"
	SET TARGET TO LandingZone.
	
if vehicle_sub_type = "SN20-Profile"
{
	SET LandingTarget TO BODY:GEOPOSITIONLATLNG(23.12854, -159.982839).
	set thrust to 0.
	shutDownAllEngines().
	
	//DEBUG:
	PRINT "WAIT 1000".
	WAIT 1000.
}

set present_heading to SHIP:HEADING.

// SS-FLIP-MANEUVER
// --------------------------------------------------------------------------------------------
SET steeringVdeg to -1. //shipPitch.
SET steeringDir TO -(90).		// W/E
set steeringVroll to -180.		// -270 = Zero Rotation
LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//steering_falcon(Vdeg).
SET thrust TO 0.1.
WAIT 2.5.
LOG "Done" to flip.txt.

// SS-RETURN:
// --------------------------------------------------------------------------------------------
RUNPATH( "boot/Falcon-Return.c").	//RESET thrust!!!

if vehicle_type = "SN9-Profile1"
{
	SET thrust TO 0.25.
	boostback_burn(true).
	SET thrust TO 0.

	// BELLY DOWN:
	// --------------------------------------------------------------------------------------------
	SET steeringVdeg to 3. //shipPitch.
	SET steeringDir TO -(90).		// W/E
	set steeringVroll to -180.		// -270 = Zero Rotation
	LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//steering_falcon(Vdeg).

	//WAIT 30:
	// --------------------------------------------------------------------------------------------
	AG5 ON.
	FROM {local x is 30.} UNTIL x = 0 STEP {set x to x-1.} DO 
	{
		wait 1.
		PRINT_STATUS (3).
	}
	AG5 OFF.

	activateAllEngines().
	if vehicle_type = "SN9-Profile1"
		sn11_test_profile_deactivate_engine1().
	// SS-LAND:
	// --------------------------------------------------------------------------------------------
	aerodynamic_guidance().
	landingBurn(). //3000
	touchdown().
	rocketshutdown().
	after_landing().
}

if vehicle_sub_type = "SN20-Profile"
{
	if status <> "LANDED" and status <> "SPLASHED"
		main_falcon_return().
}


LOG   "END: starship_lowentry_return.c" to log.txt.
LOG   "-------------------------------" to log.txt.