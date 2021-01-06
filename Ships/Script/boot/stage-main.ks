// --------------------------------------------------------------------------------------------
// Filename: stage-main.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is used before main.c, to distribute tasks among all Processors.
// 03/Jan/2021
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
core:doaction("Open Terminal", true).
CLEARSCREEN.

// URGENT/RESET STUFF: -------------------------------------------------------
if (status = "PRELAUNCH" or status = "LANDED") and ( BODY:name = "Kerbin" or BODY:name = "Earth" )
{
	DELETEPATH("SLAVE.TXT").
	LOG  "SET SLAVE_STAGE to 0." to SLAVE.TXT.

	DELETEPATH("CIRCULARIZE.txt").
	DELETEPATH("LOG.txt").
	DELETEPATH("MAIN_SHIP.txt").
}
DELETEPATH("FLIP.txt"). 				//Reload, maybe?

// DEFINE MISSION PROFILE: -------------------------------------------------------
RUNPATH( "boot/sw-version.c" ).

LIST PROCESSORS IN ALL_PROCESSORS.
PRINT  "TOTAL CPU PROCESSORS: "+ALL_PROCESSORS:LENGTH.
LOG  "["+STAGE_1_TYPE+"] TOTAL CPU PROCESSORS: "+ALL_PROCESSORS:LENGTH to LOG.txt.
if ALL_PROCESSORS:LENGTH = 0 { WAIT 5. reboot. }

// Identify other processors from (Booster(s), Stage-1 and Stage-2) ----------------
declare global PROCESSOR_STAGE1 to ALL_PROCESSORS[0].
declare global PROCESSOR_STAGE1L to ALL_PROCESSORS[0].
declare global PROCESSOR_STAGE1R to ALL_PROCESSORS[0].
declare global PROCESSOR_STAGE2 to ALL_PROCESSORS[0].
declare global PROCESSOR_MAINID to ALL_PROCESSORS[0].
declare global PROCESSOR_LEMID to ALL_PROCESSORS[0].

PRINT "List CPU Stages:".
 FROM {local counter is 0.} UNTIL counter = ALL_PROCESSORS:LENGTH STEP {SET counter to counter + 1.} DO {
	LOG  "["+STAGE_1_TYPE+"] BOOTFILENAME: "+ALL_PROCESSORS[counter]:BOOTFILENAME to LOG.txt.
	
	if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-boosters.ks") <> -1 			// STAGE-1: CORE
	{
		PRINT "- STAGE1-ID: "+counter.
		LOG  "["+STAGE_1_TYPE+"] - STAGE1-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE1 to ALL_PROCESSORS[counter].
	} 
	else if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-boosters-L.ks") <> -1 	// STAGE-1L:
	{
		PRINT "- STAGE1L-ID: "+counter.
		LOG  "["+STAGE_1_TYPE+"] - STAGE1L-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE1L to ALL_PROCESSORS[counter].
	}
	else if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-boosters-R.ks") <> -1 	// STAGE-1R:
	{
		PRINT "- STAGE1R-ID: "+counter.
		LOG  "["+STAGE_1_TYPE+"] - STAGE1R-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE1R to ALL_PROCESSORS[counter].
	}
	else if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-st2.ks") <> -1 			// STAGE-2: (Deorbit)
	{
		PRINT "- ST2-ID: "+counter.
		LOG  "["+STAGE_1_TYPE+"] - ST2-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE2 to ALL_PROCESSORS[counter].
	}
	else if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot.ks") <> -1 				// MAIN MISSION (Launch, Orbit & Deploy)
	{
		PRINT "- MAIN-ID: "+counter.
		LOG  "["+STAGE_1_TYPE+"] - MAIN-ID: "+counter to LOG.txt.
		set PROCESSOR_MAINID to ALL_PROCESSORS[counter].
	}
	else if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-lem.ks") <> -1 				// MAIN MISSION (Launch, Orbit & Deploy)
	{
		PRINT "- LEM-ID: "+counter.
		LOG  "["+STAGE_1_TYPE+"] - LEM-ID: "+counter to LOG.txt.
		set PROCESSOR_LEMID to ALL_PROCESSORS[counter].
	}
}

// SELECT LANDING ZONE: -------------------------------------------------------
if vehicle_type = "Falcon Heavy"
{
	// 1=LandZone:    LZ-1
	// 2=LandZone:    LZ-2
	// 100=DroneShip: OCISLY
	// 101=DroneShip: JRTI
	// 102=DroneShip: OCISLY-FAROUT
	IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(102)	WAIT 0.1. //OCISLY_FAROUT
	IF PROCESSOR_STAGE1L:CONNECTION:SENDMESSAGE(1)  WAIT 0.1.//LZ-1(Slave)
	IF PROCESSOR_STAGE1R:CONNECTION:SENDMESSAGE(2)  WAIT 0.1. //LZ-2(Master)
} 
else if vehicle_type = "Crew Dragon 2"
{
	if STAGE1_LAND_ON = "LAND" {
		IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(1)   WAIT 0.1.	//LZ-1   F9 LandingZone
	} else {
		IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(101) WAIT 0.1.	//JRTI
	}
} 
else if vehicle_type = "F9v1.2B5"
{
	if STAGE1_LAND_ON = "LAND" {
		IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(1)   WAIT 0.1.	//LZ-1   F9 LandingZone
	} else {
		IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(100) WAIT 0.1.	//OCISLY F9 DroneShip
	}
}

RUNPATH( "boot/main.c" ).