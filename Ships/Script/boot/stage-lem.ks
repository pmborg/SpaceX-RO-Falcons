// --------------------------------------------------------------------------------------------
// Filename: stage-lem.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to land on the a body (moon for example) and orbit again
// 13/Aug/2021
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
declare global LOG_FILE to "lem-log.txt". DELETEPATH(LOG_FILE).
runpath("boot/sw-version.c").
set STAGE_1_TYPE to "LEM".

runpath("boot/useful-tools.c").
runpath("boot/lib_warp.c").

// --------------------------------------------------------------------------------------------
Function UNDOCK_LEM
{
	SET MAPVIEW TO FALSE. wait 1.
	set ShipDockingPortPart to LEMPortGetter(SHIP, "none").
	SET docked_node TO ShipDockingPortPart:CHILDREN[0].
	IF ShipDockingPortPart:PARENT:name = ShipDockingPortPart:name {
		SET docked_node TO ShipDockingPortPart:PARENT.
	}
	SET docked_mod TO docked_node:GETMODULE("ModuleDockingNode").    
	IF docked_mod:HASACTION("undock node") {
		docked_mod:DOACTION("undock node", true).
	}
}

// --------------------------------------------------------------------------------------------
function PREPARE_FOR_LANDING
{
	clearscreen. print " ". print " ".
	update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
	UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.

	LOCK STEERING TO retrograde.
	RCS ON. WAIT 1.
	STAGE. WAIT 1.

	AG6 OFF. wait 1.
	AG6 ON. wait 1.
	SET MAPVIEW TO TRUE. wait 1.
	set warp to 3. wait 1.
	set warp to 4. wait 1.
	wait until eta:periapsis < 1000.
	set warp to 2.
	wait until eta:periapsis < 100.
	SET MAPVIEW TO FALSE.
	//
	LOCK STEERING TO retrograde.
	wait 10.
}

// --------------------------------------------------------------------------------------------
function PREPARE_FOR_CIRCULARIZE
{
	activateLEMengine().
	LOCK STEERING TO retrograde.
	lock throttle to thrust.
	set thrust to 1.

	//CIRCULARIZE 100km
	set current_altitude to periapsis.
	WAIT UNTIL periapsis <=  100000.
	set thrust to 0.
	
	//CIRCULARIZE 50km
	set warp to 3.
	wait until eta:periapsis < 120.
	set warp to 0.
	RCS ON.
	LOCK STEERING TO retrograde.
	lock throttle to thrust.
	set thrust to 1.
	WAIT UNTIL periapsis <=  50000.
	set thrust to 0.
}

// --------------------------------------------------------------------------------------------
function LUNAR_APPROACH
{
	update_phase_title("LEM LUNAR (horizontal burn)", 0, true).
	//LAND AT: 23 12 37S
	//LAND AT: 12 53 44W
	declare global LandingTarget to latlng(-23.210278, -12.895556).
	declare global LandingZone to SHIP.
	runpath("boot/common.c").
	RUNPATH( "boot/Falcon-Return.c").

	RCS ON.
	steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset).
	boostback_burn().
}

// --------------------------------------------------------------------------------------------
// START:
// --------------------------------------------------------------------------------------------
update_phase_title("[SWITCH CREWS]", 1, true).
PRINT "PRESS [ENTER] WHEN READY...". 
set ch to terminal:input:getchar().

UNDOCK_LEM().
PREPARE_FOR_LANDING().
PREPARE_FOR_CIRCULARIZE().
LUNAR_APPROACH().

//TOBE DONE...
print "DONE".
WAIT UNTIL FALSE.
