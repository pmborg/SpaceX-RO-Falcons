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
// 16/Nov/2020
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
core:doaction("Open Terminal", true).
DELETEPATH("LOG.txt").
CLEARSCREEN.
RUNPATH( "boot/sw-version.c" ).
PRINT "MODEL: "+SHIP:name.

LIST PROCESSORS IN ALL_PROCESSORS.
PRINT  "TOTAL CPU PROCESSORS: "+ALL_PROCESSORS:LENGTH.
LOG  "TOTAL CPU PROCESSORS: "+ALL_PROCESSORS:LENGTH to LOG.txt.
if ALL_PROCESSORS:LENGTH = 0 { WAIT 5. reboot. }

// Identify other processors from (Booster(s), Stage-1 and Stage-2) ----------------
declare global PROCESSOR_MAINID to ALL_PROCESSORS[0].
declare global PROCESSOR_STAGE1 to ALL_PROCESSORS[0].
declare global PROCESSOR_STAGE1L to ALL_PROCESSORS[0].
declare global PROCESSOR_STAGE1R to ALL_PROCESSORS[0].

PRINT "List CPU Stages:".
 FROM {local counter is 0.} UNTIL counter = ALL_PROCESSORS:LENGTH STEP {SET counter to counter + 1.} DO {
	
	if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-boosters.ks") <> -1 // STAGE-1:
	{
		PRINT "- STAGE1-ID: "+counter.
		LOG   "- STAGE1-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE1 to ALL_PROCESSORS[counter].
	} 
	if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-boosters-L.ks") <> -1 // STAGE-1L:
	{
		PRINT "- STAGE1L-ID: "+counter.
		LOG   "- STAGE1L-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE1L to ALL_PROCESSORS[counter].
	}
	if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot-boosters-R.ks") <> -1 // STAGE-1R:
	{
		PRINT "- STAGE1R-ID: "+counter.
		LOG   "- STAGE1R-ID: "+counter to LOG.txt.
		set PROCESSOR_STAGE1R to ALL_PROCESSORS[counter].
	}
	else if ALL_PROCESSORS[counter]:BOOTFILENAME:FIND("boot.ks") <> -1 // STAGE-2:
	{
		PRINT "- MAIN-ID: "+counter.
		LOG   "- MAIN-ID: "+counter to LOG.txt.
		set PROCESSOR_MAINID to ALL_PROCESSORS[counter].
	}
}

if vehicle_type = "Falcon Heavy"
{
	// 1=LandZone:  LZ-1
	// 2=LandZone:  LZ-2
	// 100=DroneShip: OCISLY
	// 101=DroneShip: JRTI
	IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(100)	//OCISLY
	  WAIT 0.1.

	IF PROCESSOR_STAGE1L:CONNECTION:SENDMESSAGE(1) //LZ-1(Slave)
	  WAIT 0.1.

	IF PROCESSOR_STAGE1R:CONNECTION:SENDMESSAGE(2) //LZ-2(Master)
	  WAIT 0.1.

} else
if vehicle_type = "Crew Dragon 2"
{
	IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(101) //JRTI
	  WAIT 0.1.	
} else
if vehicle_type = "F9v1.2B5"
{
	IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(100)	//OCISLY
	  WAIT 0.1.	
}


RUNPATH( "boot/main.c" ).