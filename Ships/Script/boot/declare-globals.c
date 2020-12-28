// --------------------------------------------------------------------------------------------
// Filename: declare-globals.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is called by main processor @start, to init or restart the all Mission.
// 26/Dez/2020
// --------------------------------------------------------------------------------------------
function ResetMission 
{
	DELETEPATH("target.c").
	DELETEPATH("shipmass.txt").
	DELETEPATH("booster.txt").
	DELETEPATH("orbit.txt").
	DELETEPATH("normal.txt").
	DELETEPATH("resources.txt").
	DELETEPATH("bodys_list.txt").
	WAIT 1.
}

if status = "PRELAUNCH"
	update_phase_title("PRE-LAUNCH",0, false).

// For Kerbin:
set BilliardSolarSystem to false.	// Default: False
set OuterKerbinMod to false.		// Default: False
set OuterPlanetsMod to false.		// Default: False
set Plod2Mod to false.				// Default: False
set ExtraSolarMod to false.			// Default: False
// For Earth:
set RealExoplanets to false.		// Default: False
set RealSolarSystemMod to false.	// Default: False

//Check for MODS: http://ksp-kos.github.io/KOS_DOC/
LIST BODIES IN bodylist.
declare global DEFAULT_KSC to "Kerbin".

FROM { local counter is 0. } UNTIL counter = bodylist:LENGTH STEP {SET counter to counter + 1.} DO 
{
	// Check Planetary mods, for Kerbin:
	IF bodylist[counter]:NAME = "Drip"
		set BilliardSolarSystem to true.
	IF bodylist[counter]:NAME = "Phybi"
		set OuterKerbinMod to true.
	IF bodylist[counter]:NAME = "Plock"
		set OuterPlanetsMod to true.
	IF bodylist[counter]:NAME = "Plod II"
		set Plod2Mod to true.
	IF bodylist[counter]:NAME = "Valentine"
		set ExtraSolarMod to true.
		
	// Check Planetary mods, for Earth:
	IF bodylist[counter]:NAME = "TRAPPIST-1"
		set RealExoplanets to true.
	IF bodylist[counter]:NAME = "Earth"
		set RealSolarSystemMod to true.

	LOG "NAME: "	+bodylist[counter]:NAME to BODYs_LIST.txt.
	if counter > 0
		LOG "ALTITUDE: "+bodylist[counter]:ALTITUDE/1000000 to BODYs_LIST.txt.
	LOG "DESC: "	+bodylist[counter]:DESCRIPTION to BODYs_LIST.txt.
	LOG "MASS: "	+bodylist[counter]:MASS to BODYs_LIST.txt.

	if bodylist[counter]:HASOCEAN = "True" and bodylist[counter]:HASSOLIDSURFACE = "True"
		LOG "LAND & OCEAN"to BODYs_LIST.txt.
	else {
		if bodylist[counter]:HASOCEAN = "True"
			LOG "OCEAN"to BODYs_LIST.txt.
		if bodylist[counter]:HASSOLIDSURFACE = "True"
			LOG "LAND" to BODYs_LIST.txt.
	}

	LOG "ATM: "+bodylist[counter]:ATM:HEIGHT/1000+" km" to BODYs_LIST.txt.
	LOG  "---------------------------------------------------------" to BODYs_LIST.txt.
}

if ( RealSolarSystemMod )
	set DEFAULT_KSC to "Earth".	
	
if status = "PRELAUNCH" and BODY:name = DEFAULT_KSC // first reboot? Reset State  
	ResetMission().

// else {
	// if STATUS <> "FLYING" and STATUS <> "SUB_ORBITAL"
	// {
		// PRINT "Confirm: RESET Mission? (y/n)". set ch to terminal:input:getchar().
		// if (ch = "y" OR ch = "Y")
			// ResetMission().
	// }
// }

//Note: After ResetMission()
LIST PARTS IN partsList.
FOR part IN partsList {
	//if part:MASS > 1 
	{
		//PRINT part:name + " , " + round(part:MASS,1).
		LOG part:name + " , " + round(part:MASS,1) to shipmass.txt.
	}
}

//Note: After ResetMission()
//DEBUG-INFO:
DELETEPATH("MODLIST.TXT").
declare global TOTAL_PARTS to 0.
FOR P IN SHIP:PARTS {
	LOG ("MODULES FOR PART NAMED " +P:NAME) TO MODLIST.TXT.
	LOG P:MODULES TO MODLIST.TXT.
	SET TOTAL_PARTS to TOTAL_PARTS + 1.
}.

// LIST ALL BODIES IN KerbalDestiny vector:
SET KerbalDestiny to BODY("Sun"):ORBITINGCHILDREN. //Mercury, Venus, Earth...
// Insert our Mun/Moon:
if ( RealSolarSystemMod )
	KerbalDestiny:INSERT (0,BODY("Moon")).
else
	KerbalDestiny:INSERT (0,BODY("Mun")).

PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".
PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".PRINT " ".

if NOT EXISTS("target.c") {
	set Index to 15.
	
	if RealSolarSystemMod = true
		PRINT "[X] RealSolarSystem Mod " at (16, -4+Index).
	else
		PRINT "[ ] RealSolarSystem Mod " at (16, -4+Index).
	if RealExoplanets = true		
		PRINT "[X] RealExoplanets Mod" at (16, -3+Index).
	else
		PRINT "[ ] RealExoplanets Mod" at (16, -3+Index).
	
	PRINT "SELECT TARGET:" at (0, -1+Index).
	PRINT "ATM:ALT" at (16, -1+Index).
	PRINT "1000xKm" at (25, -1+Index).
	PRINT "mass." 	at (34, -1+Index).
	
	set TOTAL_COUNTER to 0.
	FROM {local counter is 0.} UNTIL counter = KerbalDestiny:LENGTH STEP {SET counter to counter + 1.} DO 
	{
			if counter < 10
				PRINT "0"+counter+" - "+KerbalDestiny[counter]:NAME at (0, counter+Index).
			else
				PRINT counter+" - "+KerbalDestiny[counter]:NAME at (0, counter+Index).
		
			PRINT (BODY(KerbalDestiny[counter]:NAME):ATM:HEIGHT/1000)+" km" 				at (16, counter+Index).
			PRINT ROUND (BODY(KerbalDestiny[counter]:NAME):ALTITUDE/1000/1000) 				at (25, counter+Index).
			PRINT ROUND (BODY(KerbalDestiny[counter]:NAME):MASS/BODY(DEFAULT_KSC):MASS,4) 	at (34, counter+Index).
			set TOTAL_COUNTER to counter.
	}

	set ch10 	to terminal:input:getchar(). PRINT ch10.
	set ch1 	to terminal:input:getchar(). PRINT ch1.
	set chnum 	to ch10:tonumber()*10 + ch1:tonumber().
	
	LOG "declare global mission_target to KerbalDestiny["+chnum+"]." to target.c.
	if chnum >5
		LOG "declare global PlanetOuter to -1." to target.c.
	else
		LOG "declare global PlanetOuter to 1." to target.c.
}

//set KerbalDestiny[0] to BODY:name.
runpath("target.c").
declare global mission_origin to BODY:name. //string-name
PRINT "ORIGIN: "+ mission_origin.			//string-name
PRINT "TARGET: "+ mission_target:name.
PRINT "PlanetOuter: "+ PlanetOuter.

//LIBS: (LOAD After: mission_target)-------------------------------
runpath("boot/lib_activateVessel.c").
runpath("boot/lib_atmosphere_break.c").
runpath("boot/lib_manuevers.c").
runpath("boot/lib_resources.c").
runpath("boot/lib_trajectory.c").
runpath("boot/lib_wait_AP_PE.c").
runpath("boot/lib_warp.c").
//OUTBOUND --------------------------------------------------------
runpath("boot/lib_body_travel_data_lanch.c").

declare global IS_INTER_PLANETARY_MISSION to (phaseAngle < 999).	// Out of Kerbin or Earth ?
declare global IS_MOONS_MISSION to (phaseAngle = 999).				// Mun or Moon mission?
