// --------------------------------------------------------------------------------------------
// Filename: BURN.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Control the Manouver and prograde burn into: mission_target
// 28/Nov/2020
// --------------------------------------------------------------------------------------------


print "|BURN.c| "+STATUS.

function DO_BURN {
	
	set SASMODE to "STABILITY".
	SAS OFF.	//!
	AG5 ON.		
	GEAR OFF.	// Make sure: gear retracted
	BRAKES OFF.	// Air Breaks off

	if STATUS = "ORBITING" {
		PRINT "Press: y to to Confirm the BURN!". 
		PRINT "Press: n to SKIT IT".
		set ch to terminal:input:getchar(). print "selected: "+ch.
		if (ch = "n" OR ch = "N")
			 return.
		//shutDownAllEngines(). activateNucEngine().
		RUNPATH( "boot/PhaseI-Burn.c", mission_target:name ). // 1st Burn: [NOW2]
		shutDownAllEngines().
	}

	if IS_INTER_PLANETARY_MISSION {
		RUNPATH( "boot/PhaseI-Warp.c" ).	 			 // Warp out of "Kerbin" and all Moons...
		
		PRINT "Press [ENTER] to Confirm STEP!!!!!". set ch to terminal:input:getchar().
		RUNPATH( "boot/PhaseI-Transfer.c", "RADIALIN" ). // Fine Tune: [NOW4]
		shutDownAllEngines().
	}

	shutDownAllEngines().
	if (Orbit:periapsis > MAX(40000, 1.5*BODY:atm:height)) // 40,000m safe altitude (periapsis wait) in all planets.
		wait_until_periapsis().

	RCS ON.
	wait 15.
	RCS OFF.
		
	// shutDownAllEngines().
	// activateNucEngine().
	RUNPATH( "boot/PhaseII-Circularize.c", mission_target ).
}

DO_BURN().