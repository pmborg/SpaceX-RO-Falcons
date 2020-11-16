// --------------------------------------------------------------------------------------------
// Filename: stage-boosters.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to control (ST-1) Boosters and waiting phases and prepare them to land.
// 16/Nov/2020
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
RCS OFF.
RUNPATH( "boot/sw-version.c" ).
PRINT "BOOSTERS - STAGE 1 - Loading...".			//FOR STAGE-1
runpath("boot/lib_activateVessel.c").
LIST PROCESSORS IN ALL_PROCESSORS.
PRINT "ALL_PROCESSORS:LENGTH "+ALL_PROCESSORS:LENGTH.
if ALL_PROCESSORS:LENGTH = 0 { WAIT 5. reboot. }	//Check Processors Sanity-Check

update_phase_title("WAIT4LANDING TARGET", 0, true).

//Define Offline (defautl) Values: https://www.fcc.gov/media/radio/dms-decimal
declare global offline_LandingZone1 to latlng(28.612903103981047, -80.619702436818869).
declare global offline_LandingZone2 to latlng(28.612890963687462,  -80.621613149083217).

//Define VESSELS on SAVED LOAD GAME: ""
set LZ_1 to "LandingZone1". 				
set LZ_2 to "LandingZone2". 				//R: MASTER
set OCISLY to "OCISLY".				
set JRTI to "JRTI".

//Define Globals:
declare global mission_target to BODY.						// Planet Name - Sample: Kerbin, Earth
declare global LandingZone TO VESSEL(JRTI).					// VESSEL NAME - Sample: LandingZone1, LandingZone2
declare global LandingTarget TO LandingZone:GEOPOSITION.	// Landing Target Geoposition

// first reboot on LAUNCHPAD ? -> Reset/Init State:
if status = "PRELAUNCH" and ( BODY:name = "Kerbin" or BODY:name = "Earth" )
{
	DELETEPATH("1:/STAGE1_TARGET_FILE.c").

	//WAIT until signal received:
	update_phase_title("WAIT TO STARTUP SIGNAL   ", 0, true).
	WAIT UNTIL NOT CORE:MESSAGES:EMPTY.
	SET RECEIVED TO CORE:MESSAGES:POP.
	set TARGET_N to RECEIVED:CONTENT.
	PRINT "TARGET_N: "+TARGET_N.
	
	//Select target for later:
	if TARGET_N = 100
	{
		set LandingZone TO VESSEL(JRTI).
		LOG "declare global LandingZone TO VESSEL(JRTI)." to "1:/STAGE1_TARGET_FILE.c".
		LOG "INIT TARGET: VESSEL(JRTI)." to LOG.txt.			
		PRINT "LANDING AT: "+JRTI.
	} else
	if TARGET_N = 101
	{
		set LandingZone TO VESSEL(OCISLY).
		LOG "declare global LandingZone TO VESSEL(OCISLY)." to "1:/STAGE1_TARGET_FILE.c".
		LOG "INIT TARGET: VESSEL(OCISLY)." to LOG.txt.
		PRINT "LANDING AT: "+OCISLY.
	} else
	if TARGET_N = 1
	{
		set LandingZone TO VESSEL(LZ_1).
		LOG "declare global LandingZone TO VESSEL(LZ_1)." to "1:/STAGE1_TARGET_FILE.c".
		LOG "INIT TARGET: VESSEL(LZ_1)." to LOG.txt.
		PRINT "LANDING AT: "+LZ_1.
	} else
	if TARGET_N = 2
	{
		set LandingZone TO VESSEL(LZ_2).
		LOG "declare global LandingZone TO VESSEL(LZ_2)." to "1:/STAGE1_TARGET_FILE.c".
		LOG "INIT TARGET: VESSEL(LZ_2)." to LOG.txt.
		PRINT "LANDING AT: "+LZ_2.
	}
} else  {
	PRINT "LOAD: STAGE1_TARGET_FILE.c".
	// MASTER:
	if STAGE_1_TYPE <> "SLAVE" 
	{
		runpath("1:/STAGE1_TARGET_FILE.c"). //1: = Use KOS_CPU Internal Disk. (to allow each booster have it's file)
		set LandingTarget TO LandingZone:GEOPOSITION.
		PRINT "LANDING ZONE (LOADED FROM INT. DISK): "+LandingZone:NAME.
		LOG STAGE_1_TYPE+" "+LandingZone to LOG.txt.
		
		update_phase_title("BOOSTER SLEEPING...    ", 0, true).
	} else 
	// SLAVE:
	{	
		if CORE:BOOTFILENAME:FIND("boot-boosters-L.ks") > -1 		// STAGE-1L
		{
			declare global LandingZone_NAME to "LandingZone1".
			set LandingTarget TO offline_LandingZone1.
		}
		else if CORE:BOOTFILENAME:FIND("boot-boosters-R.ks") > -1 	// STAGE-1R 
		{
			declare global LandingZone_NAME to "LandingZone2".
			set LandingTarget TO offline_LandingZone2.
		}
		else if CORE:BOOTFILENAME:FIND("boot-boosters.ks") > -1 	// STAGE-1C
		{
			declare global LandingZone_NAME to "JRTI".
			set LandingTarget TO offline_DroneShip.
		}

		PRINT "LOADED - LandingZone: "+LandingZone_NAME.
		LOG STAGE_1_TYPE+" "+LandingZone_NAME to LOG.txt.
	}
}

//FORCE TO DEBUG:
// set LandingZone TO VESSEL(JRTI).
// set LandingTarget TO LandingZone:GEOPOSITION.

// Before COMMON:
if STAGE_1_TYPE = "MASTER" 
{
	set near_vessel to getNearbyProbe().
	if (near_vessel = "")
		set STAGE_1_TYPE to "CORE".
	else {
		declare global SLAVE_BOOSTER to getNearbyProbe(). 
		declare global SLAVE_CONNECTION TO SLAVE_BOOSTER:CONNECTION.
	}
}

// Init Common (After "LOADED - LandingZone"):
runpath("boot/common.c").

//WAIT until booster is "Free" from main rocket:
WHEN ALL_PROCESSORS:LENGTH > 2 THEN
{
	LIST PROCESSORS IN ALL_PROCESSORS.
	if ALL_PROCESSORS:LENGTH > 2
		PRESERVE.
	WAIT 1.
}

// Boosters still on main rocket attached? -> WAIT for the call...
LIST PROCESSORS IN ALL_PROCESSORS.
if ALL_PROCESSORS:LENGTH > 2	
{
	//WAIT for all processors...
	update_phase_title("WAIT4ALL PROCESSORS", 0, true).
	WHEN ALL_PROCESSORS:LENGTH <=2 THEN
	{
		reboot.
		PRESERVE.
		WAIT 1.
	}

	//WAIT for SEP1 or SEP2:
	update_phase_title("WAIT FOR SEP SIGNAL", 0, true).
	WAIT UNTIL NOT CORE:MESSAGES:EMPTY.
	SET CODE_RECEIVED TO CORE:MESSAGES:POP.
	PRINT "RECEIVED MSG - CODE_RECEIVED: "+CODE_RECEIVED.
	reboot.
}
else
	core:doaction("Open Terminal", true).

// WAIT and point falcons to Target (Horizontally only)
if (SHIP:VERTICALSPEED > 1) //and KUniverse:ActiveVessel = SHIP
{
	set present_heading to SHIP:HEADING.
	
	update_phase_title("STAGE-1 SEPARATION...   ", 0, true).
	SAS OFF.
	RCS OFF.
	LOCK STEERING TO SHIP:PROGRADE  + R(0,0,180).
	WAIT 5.
	PRINT_STATUS (3).
	RCS ON.

	CLEARSCREEN.	
	update_phase_title("FLIP MANEUVER   ", 0, true).
	FROM {local x is 10.} UNTIL x = 0 STEP {set x to x-1.} DO 
	{
		if STAGE_1_TYPE = "CORE"
			LOCK STEERING TO HEADING(270,0, -270).
		else
			steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset). // Calculate: impactDist
		
		PRINT_STATUS (3).
		WAIT 1.
		if ship:heading>265 and ship:heading<275 
			set x to 0.
	}
}

set TOTAL_PARTS to 0.
FOR P IN SHIP:PARTS
	SET TOTAL_PARTS to TOTAL_PARTS + 1.

// Open Inf. Thread to read values from Master:
if STAGE_1_TYPE = "SLAVE" 
{
	set y to 3.
	PRINT "[Slav]" at (44,1).
	WHEN TRUE THEN
	{
		WAIT UNTIL NOT SHIP:MESSAGES:EMPTY.		//VESSEL:QUEUE
		SET MESSAGE TO SHIP:MESSAGES:POP.
		
		SET COM_thrust to MESSAGE:CONTENT[0].
		SET COM_steeringDir to MESSAGE:CONTENT[1].
		SET COM_pitch to MESSAGE:CONTENT[2].
		SET COM_ADDONS_TR_IMPACTPOS to MESSAGE:CONTENT[3].
		SET COM_altitude to MESSAGE:CONTENT[4].
		
		//DEBUG:
		// PRINT "COM_thrust:      "+ROUND (COM_thrust,1)+"   " at (0,y+12).
		// PRINT "COM_steeringDir: "+ROUND (COM_steeringDir,1)+"   " at (0,y+13).
		// PRINT "COM_pitch:       "+ROUND (COM_pitch,1)+"   " at (0,y+14).
		// PRINT "COM_ADDONS_TR_IMPACTPOS:HEADING: "+ROUND (COM_ADDONS_TR_IMPACTPOS:HEADING,1)+"   " at (0,y+15).
		// PRINT "COM_impactDist:       "+ROUND (COM_impactDist,1)+"   " at (0,y+16).
		// PRINT "COM_targetDir:       "+ROUND (COM_targetDir,1)+"   " at (0,y+17).

		PRESERVE.
	}
} 
else {
	 if STAGE_1_TYPE <> "SLAVE" 
	 {
		UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
			SET TARGET TO LandingZone.
	 }
	 else
			set LandingTarget TO offline_LandingZone1.
}

if CORE:BOOTFILENAME:FIND("boot-boosters.ks") > -1
{
	update_phase_title("(THRUST MAX)  ", 0, true).
	engines_thrustlimit_to (100).	//Back to 100% now
}

//Activate 3 engines:
update_phase_title("(ACTIVATE 3 ENGINES)  ", 0, true).
activate3engines().

RUNPATH( "boot/Falcon-Return.c").

// After Landing
CLEARSCREEN.
set AFTER_LAND_TOTAL_PARTS to 0.
FOR P IN SHIP:PARTS 
	SET AFTER_LAND_TOTAL_PARTS to AFTER_LAND_TOTAL_PARTS + 1.

if TOTAL_PARTS = AFTER_LAND_TOTAL_PARTS and STATUS <> "SPLASHED"
	PRINT "GRATZ: Another Booster landed safely!".
else
	PRINT "INTEGRITY: "+(AFTER_LAND_TOTAL_PARTS/TOTAL_PARTS)*100+"%".