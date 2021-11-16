// --------------------------------------------------------------------------------------------
// Filename: Phase-Angle.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is to ...
// 15/Nov/2021
// --------------------------------------------------------------------------------------------

parameter destBod. 		// set destBod to BODY(mission_target).


Function CurrentPhaseAngleFinder {

  Parameter TargetPlanet is destBod.
  Parameter StartingBody is ship:body.
  Parameter ReferenceBody is sun.

  local CurrentPhaseAngle is vang(TargetPlanet:position - ReferenceBody:position, StartingBody:position - ReferenceBody:position).
  local vcrsCurrentPhaseAngle is vcrs(TargetPlanet:position - ReferenceBody:position, StartingBody:position - ReferenceBody:position).
  if vdot(v(0,1,0), vcrsCurrentPhaseAngle) <= 0 {
    set CurrentPhaseAngle to 360 - CurrentPhaseAngle.
  }

  return CurrentPhaseAngle.
}

PRINT "Process Phase Angle? (y/n)". set ch to terminal:input:getchar().
PRINT ch. wait 2.
if (ch = "y" OR ch = "Y")
{
	clearscreen.
	SET MAPVIEW TO TRUE.  	// goto: map view

	set diff to 999.
	set curangle to 0.
	set warp to 7.
	set y to 0.
	until diff < 0.1 
	{
		// -------------------------------------------------------------------------------------------------
		// SOURCE: 		https://www.reddit.com/r/Kos/comments/8z2aoe/need_some_help_with_true_anomaly_and_phase_angles/
		// VALIDATION: 	http://clowder.net/hop/railroad/EMa.htm
		
		//V2:
		set curangle to CurrentPhaseAngleFinder().
		
		//V1:
		//set curangle to vang(destBod:position-sun:position,body:position-sun:position).
		set vcrsCurrentPhaseAngle to vcrs(destBod:position-sun:position,body:position-sun:position).
		
		// FIXED-FROM-ORIGINAL:
		if vdot(v(1,1,1), vcrsCurrentPhaseAngle) <= 0 {
			if phaseAngle < 0{
				//set curangle to -curangle.		
				set diff to ABS(phaseAngle-curangle).
			}
			else {
				//set curangle to 360 - curangle.
				set diff to ABS(curangle-phaseAngle).
			}
		} else {
			if phaseAngle < 0{
				//set curangle to -360 +curangle.
				set diff to ABS(phaseAngle-curangle).
			}
			else {
				// set curangle to -curangle. ?? need to be tested!
				set diff to ABS(curangle-phaseAngle).
			}		
		}

		print "Target Phase Angle: "+ ROUND(phaseAngle,3) + "   " at (0, 5).
		print "Current Phase Angle: "+ ROUND(curangle,3) + "   " at (0, 6).
		print "diff(curangle-phaseAngle): "+ ROUND(diff,3) + "   " at (0, 7).
		//print "vdot: "+ vdot(v(1,1,1), vcrsCurrentPhaseAngle) + "   " at (0, 8).
		print "y: "+ y + "   " at (0, 9).

		if (diff > -10 and diff < 10) and y = 0 {
			set warp to 6.
			set y to 1.
			//wait 0.05.
		}
			
		if y = 0 wait 1.0.

	}

	set warp to 0.
	print "We are on the correct Phase Angle" at (0, 9).
	wait 15.
	//clearscreen.
	SET MAPVIEW TO FALSE.  // map view: off

	PRINT "Phase Angle, complete ".
	PRINT "Press [ENTER] to continue.". set ch to terminal:input:getchar().
	CLEARSCREEN. print " ". print " ".
}