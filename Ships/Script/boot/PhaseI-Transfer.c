// --------------------------------------------------------------------------------------------
// Filename: PhaseI-Transfer.c
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
update_phase_title("PhaseI-Transfer: START", 1, false);
parameter burnmode. 

// TRANSFER:
// ----------------------------------------------------------------
clearscreen.
SAS ON.
set sasmode to burnmode.
set thrust to 1.
lock throttle to thrust.

set cond to 0.
until cond = 1 {
	set thesepatches to ship:patches.
	print "ship:patches[1] "+thesepatches+"        " at (0,12).
	
	if ship:patches:length = 3 {
		set thrust to 0.
		break.
	}
	
	wait 0.1.
}
SAS OFF.
wait 1.

// WARP!
// ----------------------------------------------------------------
set warp to 7.
wait 1.
UNTIL ship:Orbit:TRANSITION <> "ENCOUNTER".
set warp to 0.
wait 1.
update_phase_title("PhaseI-Transfer: END", 1, false);