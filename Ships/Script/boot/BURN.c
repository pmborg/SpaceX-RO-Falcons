// --------------------------------------------------------------------------------------------
// Filename: BURN.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Control the Manouver and prograde burn into: mission_target
// 07/Jan/2021
// --------------------------------------------------------------------------------------------
CLEARSCREEN. print " ". print " ".

function DO_BURN {
	
	SAS OFF.	
	GEAR OFF.	// Make sure: gear retracted
	BRAKES OFF.	// Air Breaks off

	//BURN -------------------------------------------------------
	if STATUS = "ORBITING" {
		
		// PRINT "Press: y to to Confirm the BURN!". 
		// PRINT "Press: n to SKIT IT".
		// set ch to terminal:input:getchar(). print "selected: "+ch.
		// if (ch = "y" OR ch = "Ys")
		update_phase_title("MAIN BURN", 1, false).
		{
			RUNPATH( "boot/PhaseI-Burn.c", mission_target:name ). // MAIN BURN (to target BODY Mission)
			shutDownAllEngines().
			RCS OFF.
		}
	}

	//WARP(INTER_PLANETARY_MISSION) ----------------------------
	if IS_INTER_PLANETARY_MISSION {
		update_phase_title("PhaseI-Warp", 1, false).
		RUNPATH( "boot/PhaseI-Warp.c" ).	 			 // Warp out of "Kerbin" and all Moons...
		
		PRINT "Press [ENTER] to Confirm STEP!!!!!". set ch to terminal:input:getchar().
		RUNPATH( "boot/PhaseI-Transfer.c", "RADIALIN" ). // Fine Tune: [NOW4]
	}

	//PE -------------------------------------------------------
	update_phase_title("WARP-TO-PE", 1, false).
	shutDownAllEngines().
	AG5 ON. //Turn on Generators + Antenas
	if (Orbit:periapsis > MAX(40000, 1.5*BODY:atm:height)) // 40,000m safe altitude (periapsis wait) in all planets.
		wait_until_periapsis().

	if vehicle_type <> "SaturnV"
	{
		RCS ON.
		wait 15.
		RCS OFF.
	}
		
	//CIRCULARIZE TARGET -------------------------------------------------------
	update_phase_title("Circle "+mission_target, 1, false).
	RUNPATH( "boot/PhaseII-Circularize.c", mission_target ).
}

DO_BURN().