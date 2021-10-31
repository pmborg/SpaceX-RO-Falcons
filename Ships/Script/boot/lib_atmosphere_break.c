// --------------------------------------------------------------------------------------------
// Filename: lib_atmosphere_break.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This is used in orbital (de-orbit) mechanics in an atmospheric planet,
//              to land without precision (land any where near by)
// 31/Oct/2021
// --------------------------------------------------------------------------------------------

function atmosphereBreak 
{
	RCS OFF.
	// SAS ON.
	// WAIT 1.
	if maxthrust > 0	// Do we have: fuel & active engines ?
	{
		// UNLOCK STEERING.
		// WAIT 1.
		// set sasmode TO "RETROGRADE".
		// WAIT 1.
		LOCK STEERING TO retrograde.
	}

	set x to warp.
	set warp to 0.
	WAIT 1.
	//SAS OFF.
	if vehicle_type <> "Space4"
		AG5 OFF. // Painels OFF: to Avoid ship rotation
	if alt:periapsis >= (body:atm:height/3) {
		// Crude code to get it set up right to start with:
		PRINT "Periapsis is too high. Burning: retrograde to correct!".
		LOCK STEERING TO retrograde.
		WAIT 15.
		PRINT "WAIT 15.".
		lock throttle to 0.4.
		until alt:periapsis < (body:atm:height/3)
		{
			if maxthrust=0 	// no more-fuel? break
				break.
			WAIT 0.1.
		}
	}else{
		// Crude code to get it set up right to start with:
		PRINT "Periapsis is too low. Burning: prograde to correct!".
		prograde_check().
		LOCK STEERING TO prograde. wait 1.
		//PRINT "WAIT 15.".
		//WAIT 15.
		lock throttle to 0.4.
		until alt:periapsis > (body:atm:height/3)
		{
			if maxthrust=0	// no more-fuel? break
				break.
			WAIT 0.1.
		}
	}

	lock throttle to 0.
	RCS OFF.
	unlock throttle.
	unlock steering.
	
	if vehicle_type <> "Space4"
		AG5 ON. // Painels ON
	set warp to x.
}