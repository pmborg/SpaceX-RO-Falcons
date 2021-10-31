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
clearscreen.
update_phase_title("PhaseI-Transfer: START", 1, false).
WAIT 5.
parameter burnmode. 

// TRANSFER:
// ----------------------------------------------------------------
if vehicle_type = "Space4"
{
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	if  maxthrust = 0
		{ AG9 ON. wait 1.}
	if  maxthrust = 0
		{ AG9 OFF. wait 1.}
}

SAS ON. wait 0.1.
//set sasmode to burnmode. wait 0.1.
set sasmode TO "PROGRADE". wait 1.

set thrust to 1. wait 0.1.
lock throttle to thrust. wait 0.1.

set cond to 0.
update_phase_title("PhaseI-Transfer: TARGET ENCOUNTER", 1, false).
until cond = 1 {
	set thesepatches to ship:patches.
	print "ship:patches[1] "+thesepatches+"        " at (0,12).
	
	if ship:patches:length = 3 {
		
		if ship:Orbit:TRANSITION <> "FINAL" 
		{
			FROM {local i is 0.} UNTIL i = thesepatches:length-1 STEP {SET i to i + 1.} DO 
			{
				print "thesepatches[i] "+thesepatches[i]+"        " at (0,16+i).
				if thesepatches[i]:NAME = mission_target:NAME
				{
					set thrust to 0.1. wait 0.1.
					LOCK STEERING TO PROGRADE. wait 0.1. //set SASMODE to "STABILITY". wait 0.1.
					set thrust to 1. wait 0.1. //Safty Margin
					
					set thrust to 0.
					set warp to 0.
					break.
				}
			}
		}
	}
	
	wait 0.1.
}
SAS OFF.
wait 1.

// WARP!
// ----------------------------------------------------------------
update_phase_title("PhaseI-Transfer: WARP TO TARGET", 1, false).
set warp to 7.
wait 1.
UNTIL ship:Orbit:TRANSITION <> "ENCOUNTER".
set warp to 0.
wait 1.
update_phase_title("PhaseI-Transfer: END", 1, false).