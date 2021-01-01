// --------------------------------------------------------------------------------------------
// Filename: iss-orbit.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to keep ISS at a fixed and oriented Orbit (LEO) arround Earth.
// 14/Nov/2020
// --------------------------------------------------------------------------------------------
CLEARSCREEN.

PRINT "fixing iss orbit:".


if SHIP:name = "SpaceY-GateWay"
	LOCK STEERING TO HEADING(180, -90)+ R(0,0,0).
else
	LOCK STEERING TO HEADING(180, 90)+ R(0,0,0).
	
UNTIL FALSE
{
	WAIT 10.
}
