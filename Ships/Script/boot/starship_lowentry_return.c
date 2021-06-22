// --------------------------------------------------------------------------------------------
// Filename: starship_lowentry_return.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is to test the Starship Horizontal flight.
// 22/Jun/2021
// --------------------------------------------------------------------------------------------

function COMPLETE_PRINT_STATUS
{
		parameter line is 3.
		parameter th is thrust.
		
		set vsurf to velocity:surface.
		set Vsx to vsurf:x.
		set Vsy to vsurf:y.
		set Vsz to vsurf:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).	
		set vel to SQRT(Vs2).
		//update_atmosphere (altitude, vel).
		log_data (vel).
		
		PRINT_STATUS (line, th).
}		

// Init Common:
// --------------------------------------------------------------------------------------------
LOG   "START: starship_lowentry_return.c" to log.txt.
CLEARSCREEN.
set STAGE_1_TYPE to "CORE".

update_phase_title("starship_lowentry", 0, true).

runpath("boot/spacex_defaults.c").
runpath("boot/common.c").

DELETEPATH("1:/STAGE1_TARGET_FILE.c"). 	//KOS local HD
DELETEPATH("FLIP.txt"). 				//Real HD
DELETEPATH(STAGE_1_TYPE+"burn.txt").

//WAIT TO BE ACTIVE:
if KUniverse:ActiveVessel <> SHIP
	update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.

if vehicle_type = "SN9-Profile1"
	SET TARGET TO LandingZone.
	
if vehicle_sub_type = "SN16-Profile1"
{
	set vehicle_sub_type to "SN20-Profile".
	set thrust to 0.
	shutDownAllEngines().
}
	
if vehicle_sub_type = "SN20-Profile" and altitude > 140000 //140km
{
	SET LandingTarget TO BODY:GEOPOSITIONLATLNG(23.12854, -159.982839).
	set thrust to 0.
	shutDownAllEngines().
	
	//PREPARE 4 RETRO BURN:
	clearscreen.
	if altitude > BODY:ATM:HEIGHT
	{
		update_phase_title("PREPARE 4 RETRO BURN", 1).
		wait_until_periapsis().
		SAS OFF.
		lock steering to retrograde.
		AG2 OFF. wait 0.1.
		AG2 ON. wait 0.1.

		wait 20.
		update_phase_title("RETRO BURN", 1).
		set thrust to 0.1.
	}

	//DE-ORBIT BURN:
	set we_are_done to false.
	until (we_are_done)
	{
		SET prev_impactDist to impactDist.
		wait 0.1.
		COMPLETE_PRINT_STATUS (3, thrust). 	
		if impactDist < 3000000 //and impactDist > prev_impactDist)
			set we_are_done to true.
		
		set vsurf to velocity:surface.
		set Vsx to vsurf:x.
		set Vsy to vsurf:y.
		set Vsz to vsurf:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).	
		set vel to SQRT(Vs2).
		log_data (vel).
	}
}

if vehicle_type = "StarShip"
{
	//WARP TO ATM:
	update_phase_title("RETRO-DEORBIT: DONE", 1).
	set thrust to 0.  wait 0.1.
	shutDownAllEngines().
	wait 1.
	SAS OFF. wait 0.1.
	
	update_phase_title("WARP TO ATM", 1).
	set kuniverse:timewarp:MODE to "RAILS".	wait 1. //RESET
	set warp to 2.
	
	//ENTER IN ATM:
	set we_are_done to false.
	until (we_are_done)
	{
		COMPLETE_PRINT_STATUS (3, thrust). 	
		if altitude <= BODY:ATM:HEIGHT
			set we_are_done to true.
		
		set vsurf to velocity:surface.
		set Vsx to vsurf:x.
		set Vsy to vsurf:y.
		set Vsz to vsurf:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).	
		set vel to SQRT(Vs2).
		log_data (vel).
	}
}

set present_heading to SHIP:HEADING.

if vehicle_sub_type = "SN20-Profile"
{
	update_phase_title("HI RE-ENTRY", 1).
	//unlock steering. wait 0.1.
	SAS OFF. RCS ON. wait 0.1.

	set warp to 0. wait 1.
	set kuniverse:timewarp:MODE to "PHYSICS". wait 1.//WARP with PHYSICS
	
	update_phase_title("LOW RE-ENTRY", 1).
	//wait 1. SAS ON. wait 2.
	// SET SASMODE TO "RADIALOUT". wait 1.
	// SET SASMODE TO "RADIALOUT". wait 1.
	// SET SASMODE TO "RADIALOUT". wait 1.
	// SET SASMODE TO "RADIALOUT". wait 1.
	LOCK STEERING TO UP + R(0,-15,90+lat_correction). wait 0.1.
	
	set warp to 4.
	
	update_phase_title("RE-ENTRY", 1).
	until (altitude < 10000) 
	{
		COMPLETE_PRINT_STATUS (3, thrust). wait 0.1.
	}
}

// SAS OFF.
set warp to 0.

// SS-FLIP-MANEUVER
// --------------------------------------------------------------------------------------------
update_phase_title("HORIZ.MANEUVER", 1).
SET steeringVdeg to -1. 		//shipPitch.
SET steeringDir TO -(90).		// W/E
if vehicle_sub_type = "SN20-Profile"
	set steeringVroll to 0.			// 0 = Zero Rotation
if vehicle_type = "SN9-Profile1"
	set steeringVroll to -180.		// -270 = Zero Rotation
	
LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//steering_falcon(Vdeg).
if vehicle_type = "SN9-Profile1"
	SET thrust TO 0.1.
WAIT 2.5.
LOG "Done" to flip.txt.

if vehicle_sub_type <> "SN20-Profile"
{
	// SS-RETURN:
	// --------------------------------------------------------------------------------------------
	RUNPATH( "boot/Falcon-Return.c").	//RESET thrust!!!

	if vehicle_type = "SN9-Profile1"
	{
		//"SN9-Profile1":
		SET thrust TO 0.25.
		boostback_burn(true).
		SET thrust TO 0.

		// BELLY DOWN:
		// --------------------------------------------------------------------------------------------
		SET steeringVdeg to 3. 			//shipPitch.
		SET steeringDir TO -(90).		// W/E
		set steeringVroll to -180.		// -270 = Zero Rotation
		LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//steering_falcon(Vdeg).

		//WAIT 30:
		// --------------------------------------------------------------------------------------------
		AG5 ON.
		FROM {local x is 30.} UNTIL x = 0 STEP {set x to x-1.} DO 
		{
			COMPLETE_PRINT_STATUS (3). wait 1.
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
} 
else {
	// BELLY DOWN:
	// --------------------------------------------------------------------------------------------
	SET steeringVdeg to 3. 			//shipPitch.
	SET steeringDir TO -(90).		// W/E
	set steeringVroll to 0.			// 0 = Zero Rotation
	LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//steering_falcon(Vdeg).
	
	if vehicle_type = "SN16-Profile1"
		set landing_altitude to 500.
	else
		set landing_altitude to 2000.
	
	until (altitude < landing_altitude) {
		COMPLETE_PRINT_STATUS (3). wait 0.1.
	}
	update_phase_title("LAST BURN", 1).
	SAS ON. wait 0.1.
	AG2 OFF. wait 0.1.
	AG2 ON. wait 0.1.
    UNLOCk steering. wait 1.
	SET SASMODE TO "RETROGRADE". wait 1.
	//lock steering to retrograde.
	set thrust to 1.  wait 1.
	
	RUNPATH( "boot/PhaseIII-Land.c" ).
}

LOG   "END: starship_lowentry_return.c" to log.txt.
LOG   "-------------------------------" to log.txt.