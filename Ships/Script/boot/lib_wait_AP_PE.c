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
// 04/Nov/2021
// --------------------------------------------------------------------------------------------

function  warp_until_periapsis
{
	parameter w is 30.
	parameter sas_mode is "PROGRADE".
	
	update_phase_title("wait until periapsis", 0, false).
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS ON. wait 0.1.
	set sasmode TO sas_mode. wait 1.
	
	if eta:periapsis > 86400. 	//1Day
		set warp to 4.
	WAIT until eta:periapsis < 86400.

	if eta:periapsis > 600. 	//10Mins
		set warp to 3.
	WAIT until eta:periapsis < 600.

	if eta:periapsis > w. 		//30Secs
		set warp to 1.		
	WAIT until eta:periapsis < w.

	set warp to 0. WAIT 0.1.
}

function warp_until_apoapsis 
{
	parameter w is 60.	//DEFAULT: warp to, 1 min before burn.
	parameter sas_mode is "PROGRADE".
	
	update_phase_title("wait until apoapsis", 0, false).
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS ON. wait 0.1.
	set sasmode TO sas_mode. wait 1.
	
	if eta:apoapsis > 86400. //1Day
		set warp to 4.
	WAIT until eta:apoapsis < 86400.

	if eta:periapsis > 600. 	//10Mins
		set warp to 3.
	WAIT until eta:apoapsis < 600.

	if eta:apoapsis > w.
		set warp to 1.		
	WAIT until eta:apoapsis < w.

	set warp to 0. WAIT 1.
}

// WAIT WITH RCS FOR PROGRADE DIRECTION:
function prograde_check
{
	parameter sas_mode is "PROGRADE".
	
	update_phase_title("prograde wait&check", 0, false).
	
	SET MAPVIEW TO FALSE. wait 1. // map view: off
	RCS ON. wait 1.
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	set sasmode TO sas_mode. wait 1.	

	set a to ship:prograde:pitch.
	set b to ship:prograde:yaw.
	set c to ship:prograde:roll.
	lock steering to R(a,b,c).
	WAIT UNTIL ((ship:facing:pitch >= (ROUND(a) - 5) AND ship:facing:roll >= (ROUND(c) - 5)) 
		   AND (ship:facing:pitch <= (ROUND(a) + 5) AND ship:facing:roll <= ROUND(c) + 5)).
}

// WARP TOWARDS NODE:
function warp_until_node
{
	PARAMETER node.
	PARAMETER w is 30.

	update_phase_title("Warp towards node", 0, false).	
	set warp to 0. WAIT 0.1.
	
	if node:ETA > 86400. 	//1Day
		if (BODY:NAME = "Sun" or BODY:NAME = "Kerbol")
			set warp to 5.
		else
			set warp to 4.
	WAIT until node:ETA < 86400.

	if node:ETA > 600. 	//10Mins
		// if (BODY:NAME = "Sun" or BODY:NAME = "Kerbol")
			// set warp to 4.
		// else
			set warp to 3.
	WAIT until node:ETA < 600.

	if node:ETA > w. 		//30Secs
		set warp to 1.		
	WAIT until node:ETA < w.

	set warp to 0. WAIT 0.1.
}

