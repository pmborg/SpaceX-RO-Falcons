// --------------------------------------------------------------------------------------------
// Filename: PhaseI-Warp.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				...
// 31/Oct/2021
// --------------------------------------------------------------------------------------------
update_phase_title("PhaseI-Warp: START", 1, false).
clearscreen.
WarpOutofKerbinSOI().
WAIT 
update_phase_title("PhaseI-Warp: END", 1, false).