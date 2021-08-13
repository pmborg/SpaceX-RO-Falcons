// --------------------------------------------------------------------------------------------
// Filename: stage-droneship.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used for potencionally load code on droneship (JRTI or OCISLY)
// 13/Aug/2021
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
declare global LOG_FILE to "droneship-log.txt". DELETEPATH(LOG_FILE).
RUNPATH( "boot/sw-version.c" ).

PRINT "Droneship: "+SHIP_NAME.