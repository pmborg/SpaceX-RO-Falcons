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
// 04/Nov/2021
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

	UNLOCK STEERING. wait 0.1.
	SAS ON. wait 0.1.
	SET MAPVIEW TO FALSE. wait 2. set sasmode to burnmode. wait 0.1.

	if burnmode <> "PROGRADE" 
		wait 20.

	set thrust to 1. wait 0.1.
	lock throttle to thrust. wait 0.1.

	//PhaseI-Transfer: TARGET ENCOUNTER:
	//----------------------------------
	update_phase_title("PhaseI-Transfer: "+burnmode, 1, false).
	
	RCS OFF.
	
	set cond to 0.
	until cond = 1 or ((apoapsis >= mission_target:orbit:semimajoraxis) and  NOT EXISTS("transfer.txt"))
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
						set thrust to 0.
						set cond to 1.
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
runpath("boot/lib_normal.c"). //set Relative_Inclination_to_Target to getNormalOrbitAngle().

//TRANSFER:I
if NOT EXISTS("transfer.txt")
{
	change_inclination(true).	  //FORCE MATCH PLANES, IT1

	//TRANSFER:
	update_phase_title("PhaseI-Transfer: ROTATE", 1, false).
	SET MAPVIEW TO FALSE. wait 2. set sasmode to "PROGRADE". wait 0.1.
	prograde_check().
	update_phase_title("PhaseI-Transfer: BURN", 1, false).
	PhaseI_Transfer().
	LOG  "Done" to transfer.txt.
}

if NOT EXISTS("transfer2.txt")
{
	//TRANSFER:II = ENCOUNTER
	change_inclination().	//FORCE MATCH PLANES, IT2
	prograde_check().
	update_phase_title("PhaseI-Transfer: BURN-II", 1, false).
	PhaseI_Transfer().
	LOG  "Done" to transfer2.txt.
}

// WARP!
// ----------------------------------------------------------------
update_phase_title("PhaseI-Transfer: WARP TO TARGET", 1, false).
SET MAPVIEW TO FALSE. wait 1.  // map view: off
SAS OFF. wait 1.
set warp to 7. wait 1.
UNTIL ship:Orbit:TRANSITION <> "ENCOUNTER".
set warp to 0. wait 1.
update_phase_title("PhaseI-Transfer: END", 1, false).

LOG  "PhaseI-Transfer.c: done" to transfer-complete.txt.