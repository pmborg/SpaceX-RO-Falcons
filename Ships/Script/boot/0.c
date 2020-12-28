// --------------------------------------------------------------------------------------------
// Filename: 0.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to control (ST-1) Boosters and waiting phases and prepare them to land.
// 28/Dez/2020
// --------------------------------------------------------------------------------------------
RCS OFF.
RUNPATH( "boot/sw-version.c" ).
update_phase_title("STAGE 1 - Loading...", 0, false). //PRINT "Falcon 9 - STAGE 1 - Loading...".			//FOR STAGE-1
runpath("boot/lib_activateVessel.c").
LIST PROCESSORS IN ALL_PROCESSORS.
PRINT "ALL_PROCESSORS:LENGTH "+ALL_PROCESSORS:LENGTH.
if ALL_PROCESSORS:LENGTH = 0 { WAIT 5. reboot. }	//Check Processors Sanity-Check

update_phase_title("WAIT4LANDING TARGET", 0, true).
set in_sync to false.

////////////////////////////////////////////////////////////////////////////////////////////////
// INIT:
////////////////////////////////////////////////////////////////////////////////////////////////

// first reboot on LAUNCHPAD ? -> Reset/Init State:
if status = "PRELAUNCH" and ( BODY:name = "Kerbin" or BODY:name = "Earth" )
{
	DELETEPATH("1:/STAGE1_TARGET_FILE.c"). 	//KOS local HD
	DELETEPATH("FLIP.txt"). 				//Real HD

	//WAIT until signal received:
	update_phase_title("WAIT TO STARTUP SIGNAL", 0, true).
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
	if TARGET_N = 102
	{
		set LandingZone TO VESSEL(OCISLY_FAROUT).
		LOG "declare global LandingZone TO VESSEL(OCISLY_FAROUT)." to "1:/STAGE1_TARGET_FILE.c".
		LOG "INIT TARGET: VESSEL(OCISLY_FAROUT)." to LOG.txt.
		PRINT "LANDING AT: "+OCISLY_FAROUT.
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
} else {
	PRINT "LOAD: STAGE1_TARGET_FILE.c".
	// MASTER or CORE:
	if STAGE_1_TYPE <> "SLAVE" 
	{
		runpath("1:/STAGE1_TARGET_FILE.c"). //1: = Use KOS_CPU Internal Disk. (to allow each booster have it's file)
		set LandingTarget TO LandingZone:GEOPOSITION.
		PRINT "LANDING ZONE (LOADED FROM INT.DISK): "+LandingZone:NAME.
		LOG STAGE_1_TYPE+" "+LandingZone to LOG.txt.
		
		update_phase_title("BOOSTER SLEEPING...    ", 0, true).
	} else {
		// SLAVE:
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
			declare global LandingZone_NAME to "OCISLY_FAROUT".
			set LandingTarget TO offline_DroneShip.
		}

		PRINT "LOADED - LandingZone: "+LandingZone_NAME.
		LOG STAGE_1_TYPE+" "+LandingZone_NAME to LOG.txt.
	}
}

//WAIT until booster is "Free" from main rocket:
WHEN ALL_PROCESSORS:LENGTH > 1 THEN
{
	LIST PROCESSORS IN ALL_PROCESSORS.
	if ALL_PROCESSORS:LENGTH > 1
		PRESERVE.
	WAIT 0.1.
}

// Boosters still on main rocket attached? -> WAIT for the call...
LIST PROCESSORS IN ALL_PROCESSORS.
LOG  "["+STAGE_1_TYPE+"] - ALL_PROCESSORS:LENGTH: "+ALL_PROCESSORS:LENGTH to LOG.txt.
if ALL_PROCESSORS:LENGTH > 2	//Stage-1 (Have two processors)
{
	//WAIT for all processors...
	update_phase_title("WAIT4ALL PROCESSORS", 0, true).
	WHEN ALL_PROCESSORS:LENGTH <=1 THEN
	{
		LOG  STAGE_1_TYPE+" REBOOT-1" to LOG.TXT.
		reboot.
		PRESERVE.
		WAIT 1.
	}

	//WAIT for SEP1 or SEP2:
	update_phase_title("WAIT FOR SEP SIGNAL", 0, true).
	WAIT UNTIL NOT CORE:MESSAGES:EMPTY.
	SET CODE_RECEIVED TO CORE:MESSAGES:POP.
	PRINT "RECEIVED MSG - CODE_RECEIVED: "+CODE_RECEIVED.
	LOG  STAGE_1_TYPE+" REBOOT-2" to LOG.TXT.
	reboot.
}
else
	core:doaction("Open Terminal", true).

// Init Common (After "LOADED - LandingZone"):
runpath("boot/common.c").

////////////////////////////////////////////////////////////////////////////////////////////////
// START:
////////////////////////////////////////////////////////////////////////////////////////////////
set in_sync to true.

// SEPARATION
////////////////////////////////////////////////////////////////////////////////////////////////
if (SHIP:VERTICALSPEED > 1) and NOT EXISTS("flip.txt")
{
	set present_heading to SHIP:HEADING.
	
	//FLIP MANEUVER:
	flip_maneuver().
	
	if STAGE_1_TYPE = "MASTER" or STAGE_1_TYPE = "SLAVE"
	{
		if STAGE_1_TYPE = "MASTER"
			set steeringDir TO steeringDir+45.
		if STAGE_1_TYPE = "SLAVE"
			set steeringDir TO steeringDir-45.
			
		LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).//steering_falcon(Vdeg).
		WAIT 4. // wait for SEP
	}
	SET thrust TO 0.
	LOG "Done" to flip.txt.
} 

// Open Inf. Thread to read values from Master:
if STAGE_1_TYPE = "SLAVE" 
{
	set y to 3.
	update_phase_title("(INIT MSG READER)",0).
	WHEN in_sync THEN
	{
		WAIT UNTIL NOT SHIP:MESSAGES:EMPTY.		//VESSEL:QUEUE
		SET MESSAGE TO SHIP:MESSAGES:POP.
		
		//if KUniverse:ActiveVessel <> SHIP
		// {
			SET COM_thrust to MESSAGE:CONTENT[0].
			SET COM_steeringDir to MESSAGE:CONTENT[1].
			SET COM_pitch to MESSAGE:CONTENT[2].				//not used
			SET COM_ADDONS_TR_IMPACTPOS to MESSAGE:CONTENT[3].
			SET COM_altitude to MESSAGE:CONTENT[4]. 			//not used
			
			//DEBUG:
			// PRINT "COM_thrust:      "+ROUND (COM_thrust,1)+"   " at (0,y+12).
			// PRINT "COM_steeringDir: "+ROUND (COM_steeringDir,1)+"   " at (0,y+13).
			// PRINT "COM_pitch:       "+ROUND (COM_pitch,1)+"   " at (0,y+14).
			// PRINT "COM_ADDONS_TR_IMPACTPOS:HEADING: "+ROUND (COM_ADDONS_TR_IMPACTPOS:HEADING,1)+"   " at (0,y+15).
			// PRINT "COM_impactDist:       "+ROUND (COM_impactDist,1)+"   " at (0,y+16).
			// PRINT "COM_targetDir:       "+ROUND (COM_targetDir,1)+"   " at (0,y+17).
		// }
		PRESERVE.
	}
	
	set LandingTarget TO offline_LandingZone1.
} 
else 
{
	if KUniverse:ActiveVessel <> SHIP
		update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
	
	UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		SET TARGET TO LandingZone.
}

//FALCON-RETURN:
////////////////////////////////////////////////////////////////////////////////////////////////
RUNPATH( "boot/Falcon-Return.c").
if status <> "LANDED" and status <> "SPLASHED"
	main_falcon_return().

////////////////////////////////////////////////////////////////////////////////////////////////
// THE END:
////////////////////////////////////////////////////////////////////////////////////////////////