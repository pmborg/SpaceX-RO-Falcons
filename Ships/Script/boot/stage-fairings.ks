// --------------------------------------------------------------------------------------------
// Filename: stage-droneship.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used for potencionally "land"/recover the fairings
// 27/Dez/2020
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
RUNPATH( "boot/sw-version.c" ).

function display_altitude
{
	LOCK STEERING TO UP + R(0,-90-10,180+25). //UP
	PRINT "Altitude: "		 	at (0,5).	PRINT ROUND(altitude/1000,1)+" km   "		 	at (22,5).	
	WAIT 1.
}

function faring_wait_for_separation
{
	update_phase_title("WAIT FOR SEP",1, false).
	UNTIL (mass < 3) WAIT 1.
	core:doaction("Open Terminal", true).
}

function faring_descend
{
	update_phase_title("FARING DESCEND",1, false).
	SAS OFF. WAIT 1.
	RCS OFF. WAIT 1.
	UNTIL (altitude < 3000)
		display_altitude().

	update_phase_title("DEPLOY CHUTE",1, false).
	AG4 ON.
	SAS OFF. WAIT 1.
	RCS OFF. WAIT 1.
	LOCK STEERING TO UP + R(0,-90-10,180+25). //UP
	if (altitude > 2) 
		PRINT "Good Luck!".
	UNTIL (altitude < 2) 
		display_altitude().
}

faring_wait_for_separation().
core:doaction("Open Terminal", true).
faring_descend().