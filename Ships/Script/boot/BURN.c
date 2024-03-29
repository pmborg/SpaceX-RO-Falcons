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
// 08/Nov/2021
// --------------------------------------------------------------------------------------------
CLEARSCREEN. print " ". print " ".

function DO_BURN {
	
	SAS OFF.	
	GEAR OFF.	// Make sure: gear retracted
	BRAKES OFF.	// Air Breaks off

	//BURN -------------------------------------------------------
	if STATUS = "ORBITING" and (BODY:NAME = "Earth" or BODY:NAME = "Kerbin") 
	{
		
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

	//WARP(INTER_PLANETARY_MISSION) (SKIP FOR MOON) ----------------------------
	if IS_INTER_PLANETARY_MISSION {
		//update_phase_title("PhaseI-Warp", 1, false).
		if STATUS = "ORBITING" and (BODY:NAME = "Earth" or BODY:NAME = "Kerbin") {
			RUNPATH( "boot/PhaseI-Warp.c" ).	 			 // Warp out of "Kerbin" and all Moons until Sun Orbit...
			KUniverse:QUICKSAVETO("5-PhaseI-Warp done").
		}
		
		//PRINT "Press [ENTER] to Confirm - PhaseI-Transfer". set ch to terminal:input:getchar().
		RUNPATH( "boot/PhaseI-Transfer.c", "RADIALIN" ). 	// Target Orbit transfer
		KUniverse:QUICKSAVETO("7-Encounter Done").	// Encounter Done!
	}

	if (STATUS = "ESCAPING")
	{
		//INTERCEPT TARGET: MOON/MARS
		//PE -------------------------------------------------------
		update_phase_title("WARP-TO-PE", 1, true).
		
		if vehicle_type <> "Space4"
		{
			shutDownAllEngines().
			AG5 ON. //Turn on Generators + Antenas
		}
		if (Orbit:periapsis > MAX(40000, 1.5*BODY:atm:height)) // 40,000m safe altitude (periapsis wait) in all planets.
		{
			SET MAPVIEW TO FALSE. wait 0.1. set sasmode TO "retrograde". wait 0.1.	
			warp_until_periapsis().
			set warp to 0. 					//... just to make sure
			SET MAPVIEW TO FALSE. wait 0.1.	// WAIT for Refresh etc...

			if vehicle_type = "SaturnV"
			{
				UNTIL STAGE:NUMBER <=6
					{ stage. wait 5.}
				
				//TURN ON: SLM ENGINE
				update_phase_title("SLM ENGINE ON", 1, true).
				shutDownAllEngines().
				AG8 ON.			
				LOCK STEERING TO retrograde.
			}
			
			// WAIT for RETRO:
			CLEARSCREEN. print " ". print " ".
			update_phase_title("CAPTURE BURN", 1, true).
			//retrograde_check().
			lock throttle to thrust. WAIT 0.1.
			set thrust to 1. WAIT 0.1.
			RCS OFF. WAIT 0.1.
			SET MAPVIEW TO TRUE. WAIT 0.1.
			WAIT UNTIL STATUS <> "ESCAPING".
			set thrust to 0. WAIT 0.1.
			SET MAPVIEW TO FALSE. WAIT 0.1.
			//KUniverse:QUICKSAVETO("8-Break Done").
		}
	}
		
	//CIRCULARIZE TARGET -------------------------------------------------------
	if vehicle_type <> "Space4" 
	{
		update_phase_title("Circle "+mission_target:NAME, 1, true).
		RUNPATH( "boot/PhaseII-Circularize.c", mission_target ).
		KUniverse:QUICKSAVETO("9-PhaseII-Circularize Done").
	}
}

DO_BURN().