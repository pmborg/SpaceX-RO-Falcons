// --------------------------------------------------------------------------------------------
// Filename: lib_manuevers.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This is used in some speciall cases for interplanetary missions.
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

function StopSpin 
{
	PRINT "[StopSpin]".
	// Stop Spin:
	lock throttle to 0.1.
	LOCK STEERING TO up + R(0,90,0).
	RCS ON.
	FROM {local countdown is 20.} UNTIL countdown = 0 STEP {SET countdown to countdown - 1.} 
	DO {
		PRINT "stop spin:..." + countdown.
		WAIT 1.
	}
	RCS OFF.
	lock throttle to 0.0.
	unlock throttle.
	unlock steering.
	WAIT 1.
}