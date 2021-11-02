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
// 01/Nov/2021
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

function PhaseI_Transfer 
{
parameter burnmode is "PROGRADE".

	SAS ON. wait 0.1.
	set sasmode to burnmode. wait 0.1. //set sasmode TO "PROGRADE". wait 1.

	if burnmode <> "PROGRADE" 
		wait 20.

	set thrust to 1. wait 0.1.
	lock throttle to thrust. wait 0.1.

	//PhaseI-Transfer: TARGET ENCOUNTER:
	//----------------------------------
	update_phase_title("PhaseI-Transfer: "+burnmode, 1, false).
	//set cond to 0.
	//until cond = 1 or (apoapsis >= mission_target:orbit:semimajoraxis) 
	until FALSE
	{
		set thesepatches to ship:patches.
		print "burnmode: "+burnmode+"        " 							 at (0,9).
		print "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"        " at (0,10).
		print "ship:patches[1] "+thesepatches+"        " 				 at (0,12).
		
		if ship:patches:length = 3 {

			if ship:Orbit:TRANSITION <> "FINAL" 
			{
				FROM {local i is 0.} UNTIL i = thesepatches:length-1 STEP {SET i to i + 1.} DO 
				{
					print "thesepatches[i] "+thesepatches[i]+"        " at (0,16+i).
					if (thesepatches[i]:NAME = mission_target:NAME)
					{
						set thrust to 0. //set warp to 0. //LOCK STEERING TO PROGRADE. //set SASMODE to "STABILITY". wait 0.1.
						LOG "ship:patches[1] "+thesepatches to LOG_FILE.
						break.
					}
				}
			}
		}
		
		//no wait (faster as possible)
	}
	set thrust to 0. wait 1.
}
	
//Re-align orbit inclination:
//---------------------------
//PhaseI_Transfer().

//MAIN
runpath("boot/lib_normal.c").
set Relative_Inclination_to_Target to getNormalOrbitAngle().
LOG "MAIN:change_inclination" to LOG_FILE.
change_inclination().	//if (apoapsis >= mission_target:orbit:semimajoraxis)

set sasmode to "PROGRADE". wait 0.1.
prograde_check(). 		// WAIT WITH RCS FOR PROGRADE DIRECTION
PhaseI_Transfer().
	
////rep1
// LOG "rep1:change_inclination" to LOG_FILE.
// if (Relative_Inclination_to_Target > 0.1) 
	// change_inclination().

////rep2
// LOG "rep2:change_inclination" to LOG_FILE.
// if (Relative_Inclination_to_Target > 0.1) 
// change_inclination().

// WARP!
// ----------------------------------------------------------------
update_phase_title("PhaseI-Transfer: WARP TO TARGET", 1, false).
SAS OFF. wait 1.
set warp to 7. wait 1.
UNTIL ship:Orbit:TRANSITION <> "ENCOUNTER".
set warp to 0. wait 1.
update_phase_title("PhaseI-Transfer: END", 1, false).