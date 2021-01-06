// --------------------------------------------------------------------------------------------
// Filename: PhaseII-Break.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This is used in orbital (de-orbit) mechanics.
//              to land without precision (land any where near by)
// 01/Jan/2020
// --------------------------------------------------------------------------------------------

parameter goto_mission_target.

function break_velocity {

	if KUniverse:ActiveVessel = SHIP
		set TARGET to goto_mission_target.
	set warp to 0.

	set thrust to 1.
	lock throttle to thrust. WAIT 1.
	LOCK STEERING TO retrograde.
	// LOCK STEERING TO SHIP:RETROGRADE.
		
	CLEARSCREEN.
	SAS OFF.
	PRINT "PhaseII-Destroying Kinetic Energy".
	set Vo to 50.
	set X to 0.
	until Vo < 500 and SHIP:PERIAPSIS < body:atm:height
	{
		set vorbit to velocity:orbit. //Orbit
		set Vox to vorbit:x.
		set Voy to vorbit:y.
		set Voz to vorbit:z.
		set Vo to ((Vox^2)+(Voy^2)+(Voz^2))^.5.
		
		PRINT "Vo: " + ROUND(Vo) +"     " at (0,1).
		PRINT "BODY:ATM " + ROUND(body:atm:height) +"     " at (0,2).
		PRINT "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"     " at (0,3). //FINAL
		
		//SET impactDist TO calcDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS:TR:IMPACTPOS).
		//PRINT_STATUS (14).
		//WAIT 0.05.
	}
	
	set thrust to 0.
}

set vorbit to velocity:orbit.
set Vox to vorbit:x.
set Voy to vorbit:y.
set Voz to vorbit:z.
set Vo to ((Vox^2)+(Voy^2)+(Voz^2))^.5.

// LOCK STEERING TO SHIP:RETROGRADE.
RCS ON.
WAIT 12.5.
RCS OFF.
	
if Vo > 100 or Periapsis > 0 {
	break_velocity().
}
