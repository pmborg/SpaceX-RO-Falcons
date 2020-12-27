// --------------------------------------------------------------------------------------------
// Filename: stage-boosters.ks
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to control (ST-1) Boosters and waiting phases and prepare them to land.
// 13/Dez/2020
// --------------------------------------------------------------------------------------------
SWITCH TO 0.	//SWITCH TO default PATH: [KSP]/Ships/Script
CLEARSCREEN.
WAIT 1.
//Define Offline (defautl) Values: https://www.fcc.gov/media/radio/dms-decimal
declare global offline_LandingZone1 to latlng(28.612903098335448, -80.619702431884079).
declare global offline_LandingZone2 to latlng(28.612890963687462, -80.621613149083217).

//Define VESSELS on SAVED LOAD GAME: ""
declare global  LZ_1 to "LandingZone1". 				
declare global  LZ_2 to "LandingZone2". 				//R: MASTER
declare global  OCISLY to "OCISLY".
declare global  JRTI to "JRTI".
declare global  OCISLY_FAROUT to "OCISLY-FAROUT".

//Define Globals:
declare global mission_target to BODY.						// Planet Name - Sample: Kerbin, Earth
declare global LandingZone TO VESSEL(LZ_1).					// VESSEL NAME - Sample: LandingZone1, LandingZone2
declare global LandingTarget TO LandingZone:GEOPOSITION.	// Landing Target Geoposition

if CORE:BOOTFILENAME:FIND("boot-boosters-L.ks") > -1 		// STAGE-1L
{
	WAIT 1.
	runpath("SLAVE.TXT"). 	// Get "SLAVE_STAGE" Phase
	runpath("boot/"+SLAVE_STAGE+".c").
} else {
	runpath("boot/"+0+".c"). //Normal Booster
}