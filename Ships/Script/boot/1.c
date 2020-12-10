// --------------------------------------------------------------------------------------------
// Filename: 1.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to control (ST-1) Booster from FH
// 09/Dez/2020
// --------------------------------------------------------------------------------------------
RCS OFF.
SET thrust TO 0.
lock throttle to thrust.
SET SHIP:CONTROL:PILOTMAINTHROTTLE TO 0.

RUNPATH( "boot/sw-version.c" ).
set STAGE_1_TYPE to "ST-1".
runpath("boot/lib_activateVessel.c").
runpath("boot/common.c").

//Closer:
SET steeringDir TO 90.	// W/E
SET Vdeg to 90.
set Vroll to -270.		// -270 = Zero Rotation
LOCK STEERING TO HEADING(steeringDir, Vdeg, Vroll).
update_phase_title("W8-1  ", 0).
wait 10.

if KUniverse:ActiveVessel <> SHIP 
{
	update_phase_title("W8-2  ", 0).
	wait 10.
}

SET TARGET TO VESSEL("LandingZone1"). 
WAIT 1.

RUNPATH( "boot/Falcon-Return.c").
RCS ON.
steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset). // Calculate: impactDist
boostback_burn().

activateVesselProbe().

LOG  "SET SLAVE_STAGE to 2." to SLAVE.TXT.
LOG  "REBOOT:2" to LOG.TXT.
reboot.