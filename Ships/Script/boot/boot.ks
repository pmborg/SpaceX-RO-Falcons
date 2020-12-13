// --------------------------------------------------------------------------------------------
// Filename: boot.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This load wrapper avoid to re-edit and save a rocket just to change the code of
//              "boot/stage-main" file.
//				Used for main/last stage of the rocket (Falcon ST-2 or Crew Dragon 2)
// 13/Dez/2020
// --------------------------------------------------------------------------------------------

SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
runpath("boot/stage-main").