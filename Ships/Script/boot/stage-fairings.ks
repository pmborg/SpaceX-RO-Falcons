// --------------------------------------------------------------------------------------------
// Filename: stage-droneship.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used for potencionally "land" fairings
// 23/Dez/2020
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
RUNPATH( "boot/sw-version.c" ).

update_phase_title("WAIT to Deploy",1, false).
UNTIL (mass < 3) WAIT 1.

SAS OFF. WAIT 1.
RCS OFF. WAIT 1.
UNTIL (altitude < 3000)
{
	LOCK STEERING TO UP + R(0,-90-10,180+25). //UP
	PRINT "Altitude: "		 	at (0,5).	PRINT ROUND(altitude/1000,1)+" km   "		 	at (22,5).	
	WAIT 1.
}

update_phase_title("Deploy Chute",1, false).
AG4 ON.

SAS OFF. WAIT 1.
RCS OFF. WAIT 1.
LOCK STEERING TO UP + R(0,-90-10,180+25). //UP

PRINT "Good Luck!".
UNTIL (SHIP:q = 0) WAIT 1.