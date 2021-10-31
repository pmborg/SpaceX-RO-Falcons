// --------------------------------------------------------------------------------------------
// Filename: lib_wait_AP_PE.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This is used in orbital mechanics warp to AP or PE.
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

function  wait_until_periapsis
{
	parameter w is 30.
	update_phase_title("wait until periapsis", 0, false).
	
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS ON. wait 0.1.
	set sasmode TO "PROGRADE". wait 1.
	
	if eta:periapsis > 86400. 	//1Day
		set warp to 4.
	WAIT until eta:periapsis < 86400.

	if eta:periapsis > 600. 	//10Mins
		set warp to 3.
	WAIT until eta:periapsis < 600.

	if eta:periapsis > 30. 		//30Secs
		set warp to 1.		
	WAIT until eta:periapsis < w.

	set warp to 0. WAIT 0.1.
}

function wait_until_apoapsis 
{
	parameter w is 30.
	update_phase_title("wait until apoapsis", 0, false).
	set warp to 0. WAIT 1.
	
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS ON. wait 0.1.
	set sasmode TO "PROGRADE". wait 1.
	
	if eta:apoapsis > 86400. //1Day
		set warp to 4.
	WAIT until eta:apoapsis < 86400.

	if eta:apoapsis > 600. 	//10Mins
		set warp to 3.
	WAIT until eta:apoapsis < 600.

	if eta:apoapsis > 30. 	//30Secs
		set warp to 1.		
	WAIT until eta:apoapsis < w.

	set warp to 0. WAIT 1.
}