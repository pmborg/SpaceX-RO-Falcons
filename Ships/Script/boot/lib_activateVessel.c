// --------------------------------------------------------------------------------------------
// Filename: lib_activateVessel.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Activate or get near by VESSEL parts.
// 08/Dez/2020
// --------------------------------------------------------------------------------------------

declare global g_OtherBooster to SHIP.

function getNearbyProbe // The other Booster 
{
	parameter do_activation to 0.
	WAIT 2. // Give time to check main VESSEL state.
	LOCAL all_vessels IS LIST().
	LIST TARGETS IN all_vessels.
	LOG  "all_vessels:LENGTH: "+all_vessels:LENGTH to LOG.txt.
	
	//We'll want to limit ourselves to nearby vessels:
	LOCAL nearby_vessels IS LIST().
	FOR ves IN all_vessels {
		LOG "VES: "+ves:name to LOG.txt.
		if ves:name:find("Probe") > 0
		{
			// DEBUG:
			// PRINT "["+STAGE_1_TYPE+"] name: "+ves:name.
			// PRINT "["+STAGE_1_TYPE+"] mass: "+ves:mass.
			// PRINT "["+STAGE_1_TYPE+"] AIRSPEED: "+ves:AIRSPEED.
			// PRINT "["+STAGE_1_TYPE+"] Altitude: "+ves:altitude.
			
			// LOG  "["+STAGE_1_TYPE+"] name: "+ves:name to LOG.txt.
			// LOG  "["+STAGE_1_TYPE+"] mass: "+ves:mass to LOG.txt.
			// LOG  "["+STAGE_1_TYPE+"] AIRSPEED: "+ves:AIRSPEED to LOG.txt.
			// LOG  "["+STAGE_1_TYPE+"] Altitude: "+ves:altitude to LOG.txt.
			
			nearby_vessels:ADD(ves). 
			set g_OtherBooster to ves.
		}
	}

	WAIT 1. //Give time to check vessels list
	LOG  "nearby_vessels:LENGTH: "+nearby_vessels:LENGTH to LOG.txt.
	if nearby_vessels:LENGTH >= 1
		return nearby_vessels[do_activation].
	else
		return "".
}

function activateVesselProbe
{
	SET KUniverse:ACTIVEVESSEL TO VESSEL(getNearbyProbe():NAME).
}

function activateMainVessel 
{
	LOCAL all_vessels IS LIST().
	LIST TARGETS IN all_vessels.
	
	//We'll want to limit ourselves to nearby vessels:
	LOCAL nearby_vessels IS LIST().
	FOR ves IN all_vessels {
		//LOG  "["+STAGE_1_TYPE+"] name: "+ves:name to LOG.txt.
		if ves:name = MAIN_SHIP_NAME
			nearby_vessels:ADD(ves).
	}
	
	// Booster Landed, return now FOCUS-VIEW to main VESSEL:
	LOG "Activate VESSEL: "+nearby_vessels[0]:name to LOG.TXT.
	
	SET KUniverse:ACTIVEVESSEL TO VESSEL(nearby_vessels[0]:name). WAIT 2.
}