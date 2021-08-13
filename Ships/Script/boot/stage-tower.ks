// --------------------------------------------------------------------------------------------
// Filename: stage-tower.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//
// 13/Aug/2021
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
declare global LOG_FILE to "tower-log.txt". DELETEPATH(LOG_FILE).
RUNPATH( "boot/sw-version.c" ).

update_phase_title("[ ] TOWER CLEAR...", 0, false).
PRINT " ".
PRINT "mass: "+round(mass)+" t".
UNTIL (mass < 6000) WAIT 1.

update_phase_title("[ ] Counting...", 0, false).
set aim_cowntdown to 15.

PRINT " ".
PRINT "mass: "+round(mass)+" t".

FROM {local countdown is aim_cowntdown.} UNTIL countdown = 0 STEP {SET countdown to countdown - 1.} 
DO { PRINT "..." + countdown. WAIT 1. }
		
AG8 OFF.

PRINT "END".