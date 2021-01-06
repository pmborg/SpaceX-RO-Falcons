// --------------------------------------------------------------------------------------------
// Filename: 2.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to control (ST-1) Booster from FH
// 28/Dez/2020
// --------------------------------------------------------------------------------------------
RCS OFF.
RUNPATH( "boot/sw-version.c" ).
runpath("boot/lib_activateVessel.c").

set STAGE_1_TYPE to "ST-1".
runpath("boot/common.c").
RUNPATH( "boot/Falcon-Return.c").
guide_falcon_core ().