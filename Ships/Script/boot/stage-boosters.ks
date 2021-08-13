// --------------------------------------------------------------------------------------------
// Filename: stage-boosters.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to control (ST-1) Boosters and waiting phases and prepare them to land.
// 13/Aug/2021
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
declare global LOG_FILE to "booster-log.txt". DELETEPATH(LOG_FILE).

WAIT 1.
runpath("boot/spacex_defaults.c").

if CORE:BOOTFILENAME:FIND("boot-boosters-L.ks") > -1 		// STAGE-1L
{
	WAIT 1.
	runpath("SLAVE.TXT"). 	// Get "SLAVE_STAGE" Phase
	runpath("boot/"+SLAVE_STAGE+".c").
} else {
	runpath("boot/"+0+".c"). //Normal Booster
}