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
// 08/Nov/2021
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


if vehicle_type = "Space4"
{
	UNTIL NOT HASNODE { REMOVE NEXTNODE. WAIT 0.1. } //removeAllNodes!
	WAIT 1.
	//AGRESSIVE FAST VERSION: 8k delta-v 109day (out of StarShip range)
	//set BURN to NODE(TIME:seconds+39*24*60*60+12*60*60,1980.40220028894,228.638614388312,5864.88903650271).
	//CHEAP DELTA-V: 2.2k delta-v 
	//v1:
	//set BURN to NODE(TIME:seconds+13*24*60*60+9*60*60,1504.60488737683,1649.03977610003,2053.9472710979).
	//v2:
	//17d+4h
	//set BURN to NODE(TIME:seconds+17*24*60*60+4*60*60,836.616184676899,1667.15281801064,779.222569148375).
	set BURN to NODE(TIME:seconds+17*24*60*60+4*60*60,827.055927939852,1457.45023656937,779.222569148375).
	ADD BURN.
	SAS ON.
	SET MAPVIEW TO FALSE. wait 2. set sasmode to "maneuver". 	
	warp_until_node (BURN, 60*2). //warp_until_node (BURN, 60+(BURN:TIME/2)).
	wait 1.
	RCS OFF.
	execute_node (BURN, true, true, false).
}
else {
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
		change_inclination().
		
		// WAIT and force:
		set WARP to 4. WAIT 10. set WARP to 0. WAIT 1.
		LOG "part-3-change_inclination" to LOG_FILE.
		if (getNormalOrbitAngle() > 0.1) 
			change_inclination(true).
				
		prograde_check().
		update_phase_title("PhaseI-Transfer: BURN-II", 1, false).
		PhaseI_Transfer().
		LOG  "Done" to transfer2.txt.
	}
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

LOG  "PhaseI-Transfer.c: done" to transfer_complete.txt.