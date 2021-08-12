// --------------------------------------------------------------------------------------------
// Filename: lib_activateVessel.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Activate or get near by VESSEL.
// 11/Aug/2021
// --------------------------------------------------------------------------------------------

declare global g_OtherBooster to SHIP.

function getNearbyProbe // The other Booster 
{
	parameter do_activation to 0.
	parameter ship_name to "Probe".
	parameter max_distance to 99999999.
	LOG  "getNearbyProbe("+do_activation+","+ship_name+","+max_distance+")" to LOG_FILE.
	
	WAIT 0.//WAIT 2. // Give time to check main VESSEL state.
	LOCAL all_vessels IS LIST().
	LIST TARGETS IN all_vessels.
	LOG  "all_vessels:LENGTH: "+all_vessels:LENGTH to LOG_FILE.
	
	//We'll want to limit ourselves to nearby vessels:
	LOCAL nearby_vessels IS LIST().
	FOR ves IN all_vessels {
		LOG "VES: "+ves:name to LOG_FILE.
		if ves:name:find(ship_name) > 0
		{
			// DEBUG:
			// PRINT "["+STAGE_1_TYPE+"] name: "+ves:name.
			// PRINT "["+STAGE_1_TYPE+"] mass: "+ves:mass.
			// PRINT "["+STAGE_1_TYPE+"] AIRSPEED: "+ves:AIRSPEED.
			// PRINT "["+STAGE_1_TYPE+"] Altitude: "+ves:altitude.
			LOG  "["+STAGE_1_TYPE+"] name: "+ves:name to LOG_FILE.
			LOG  "["+STAGE_1_TYPE+"] mass: "+ves:mass to LOG_FILE.
			LOG  "["+STAGE_1_TYPE+"] AIRSPEED: "+ves:AIRSPEED to LOG_FILE.
			LOG  "["+STAGE_1_TYPE+"] Altitude: "+ves:altitude to LOG_FILE.
			//LOG  "["+STAGE_1_TYPE+"] ves:GEOPOSITION:DISTANCE: "+ves:GEOPOSITION:DISTANCE to LOG_FILE.
			
			nearby_vessels:ADD(ves). 
			set g_OtherBooster to ves.
		}
	}

	WAIT 0.//WAIT 1. //Give time to check vessels list
	LOG  "nearby_vessels:LENGTH: "+nearby_vessels:LENGTH to LOG_FILE.
	if nearby_vessels:LENGTH >= 1
		return nearby_vessels[do_activation].
	else
		return "".
}

function activateVesselProbe
{
	parameter do_activation to 0.
	parameter ship_name to "Probe".
	LOG  "activateVesselProbe(do_activation"+do_activation+","+ship_name+")" to LOG_FILE.	

	set vessel_near_vessel to "".
	set vessel_near_vessel to getNearbyProbe(do_activation, ship_name).
	if vessel_near_vessel <> ""
		kuniverse:forceactive(VESSEL(vessel_near_vessel:NAME)). //SET KUniverse:ACTIVEVESSEL TO VESSEL(vessel_near_vessel:NAME).
	update_phase_title("(activate-ST1)",0,false).
	WAIT 2.
}

function activateMainVessel
{
	LOG  "activateMainVessel()" to LOG_FILE.
	LOCAL all_vessels IS LIST().
	LIST TARGETS IN all_vessels.

	SET MAIN_SHIP_NAME TO MAIN_SHIP_NAME:REPLACE(" Ship", "").
	SET MAIN_SHIP_NAME TO MAIN_SHIP_NAME:REPLACE(" Probe", "").
	LOG  "MAIN_SHIP_NAME: "+MAIN_SHIP_NAME to LOG_FILE.
	
	//We'll want to limit ourselves to nearby vessels:
	LOCAL nearby_vessels IS LIST().
	FOR ves IN all_vessels {
		LOG  "["+STAGE_1_TYPE+"] name: "+ves:name to LOG_FILE.
		if ves:name = MAIN_SHIP_NAME
			nearby_vessels:ADD(ves).
	}
	
	// Booster Landed, return now FOCUS-VIEW to main VESSEL:
	LOG "Activate MAIN VESSEL: "+nearby_vessels[0]:name to LOG_FILE.
	kuniverse:forceactive(VESSEL(nearby_vessels[0]:name)). //SET KUniverse:ACTIVEVESSEL TO VESSEL(nearby_vessels[0]:name). 
	update_phase_title("(activateMainVessel)",0,false).
	WAIT 2.
}