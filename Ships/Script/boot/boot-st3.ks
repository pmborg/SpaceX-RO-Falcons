// --------------------------------------------------------------------------------------------
// Filename: boot-st3.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This load wrapper avoid to re-edit and save a rocket just to change the code of
//              "boot/stage-3-deorbit" file.
//				Used to do the deorbit of stage-3
// 01/Jan/2020
// --------------------------------------------------------------------------------------------

SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
runpath("boot/stage-3-deorbit.c").