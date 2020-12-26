// --------------------------------------------------------------------------------------------
// Filename: main.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is called by main processor to Orchestre all mission phases.
// 26/Dez/2020
// --------------------------------------------------------------------------------------------

// Reset Engine settings before all, ("migth be a reboot")
set thrust to 0.	
lock throttle to thrust.
WAIT 0.

runpath("boot/declare-globals.c").
runpath("boot/atm.c").

//ACTION: REFUEL ----------------------------------------------
if mission_origin <> DEFAULT_KSC //mission_target //ORIGIN = TARGET
{
	//CLEARSCREEN.
	update_phase_title("Confirm: START Refuel?", 0, false).
	PRINT "Confirm: START Refuel? (y/n)". set ch to terminal:input:getchar().
	if (ch = "y" OR ch = "Y") {
		traversePartsForResources("fuel").
		runpath("boot/refuel.c").
	}
}

if NOT EXISTS("resources.txt") 			// Refuelled already?, SKIP "GO-JOURNEY", goto "RETURN-JOURNEY"
{
	if (IS_INTER_PLANETARY_MISSION)
		RUNPATH( "boot/Phase Angle.c", BODY(mission_target) ).	// Warp to Correct Phase Angle

	//ACTION:  --------------------------------------------------------
	if (  (BODY:name = DEFAULT_KSC and periapsis < body:atm:height) or
	      (BODY:name <> DEFAULT_KSC and (status = "LANDED" or status = "SPLASHED"))  )
	{
		if periapsis < 0
		{
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Orbit.c", LEOrbit ).	// [1] ATM-Launch: "Launch-Orbit.c" (Have Atmosphere)
			else
				RUNPATH( "boot/PhaseIV-Orbit.c" ).			// NO-ATM-Launch: "PhaseIV-Orbit.c"
		}
		
		if (BODY:name = mission_origin) and (apoapsis < LEOrbit or periapsis < body:atm:height) or orbit_type = "GSO"
		{
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).
			else
				RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
			
			//SET ON SATELITE Solar Panels:
			//AG3 ON. //Toggle
		} else
			PRINT "SKIP: Launch-Circularize".
	} else
		PRINT "SKIP: Launch".

	// Adjust mission inclination?
	if NOT EXISTS("normal.txt")
	{
		clearscreen.
		// Just reaction-wheels Stability:
		RCS OFF.
		SAS OFF.
		LOCK STEERING TO SHIP:prograde.
		wait 1.
		// Confirm?
		if KUniverse:ActiveVessel <> SHIP {
			update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
			UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		}
		update_phase_title("Confirm: Phase-Normal?", 0, false).
		PRINT "Confirm: Phase-Normal? (y/n)" at (0,5). set ch to terminal:input:getchar().
		if (ch = "y" OR ch = "Y")
			runpath( "boot/Phase0-Normal.c", mission_target).	// Correct Normal Before Burn
		LOG "Normal" to normal.txt.
		CLEARSCREEN.
	}
	else
		PRINT "SKIP: Normal".

	//ACTION: BURN ----------------------------------------------------
	if STATUS = "ORBITING" and apoapsis > body:atm:height and periapsis > body:atm:height and BODY:name = mission_origin
		if BODY:name <> mission_target:name
			runpath("boot/BURN.c").
	else
		PRINT "SKIP: BURN".

	//ACTION: Break & LAND! -------------------------------------------
	
	if status <> "LANDED" and status <> "SPLASHED"
	{
		if EXISTS("CIRCULARIZE.txt")
			RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).
			
		if KUniverse:ActiveVessel <> SHIP {
			update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
			UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		}
		RCS OFF.
		SAS OFF.
		LOCK STEERING TO SHIP:prograde.
		wait 1.
		PRINT "Press: 1 - Abort/Land Anywhere!". 
		PRINT "Press: 2 - Stage Satellite". 
		PRINT "Press: 3 - Circularize". 
		set ch to terminal:input:getchar(). PRINT "selected: "+ch.
		if ch="1" or ch =""  {
			update_phase_title("Confirm: SPEED-BREAK?", 0, false).
			PRINT "Confirm: SPEED-BREAK? (y/n)". set ch to terminal:input:getchar().
			if (ch = "y" OR ch = "Y")
				RUNPATH( "boot/PhaseII-Break.c", mission_target ).	 // Burn Capture & Burn Circularize: [NOW5]
			
			RUNPATH( "boot/PhaseIII-Land.c" ).  	// Auto-Land / Touch-Down
		}
		else if ch="2" {
			//SEND PROCESSOR ID TO BOOSTER
			IF PROCESSOR_STAGE2:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
				{ PRINT "PROCESSOR_STAGE1: Message sent!". WAIT 1. }
				
			RCS OFF.
			SAS OFF.
			LOCK STEERING TO prograde.
			wait 1.
			
			stage. wait 1.
			SAS ON. wait 1.
			SET SASMODE TO "RADIALIN". wait 1.
			AG4 ON.
			CLEARSCREEN.
		}
		else if ch="3" {
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).
			else
				RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
			reboot.
		}
	}
	
	LOG  STAGE_1_TYPE+" REBOOT FOR RE-FUEL" to LOG.TXT.
}

//INBOUND ---------------------------------------------------------
if BODY:name <> DEFAULT_KSC {
	CLEARSCREEN.
	PRINT "Press [y/n] y to Confirm the Kerbin Return, n to new target!".set ch to terminal:input:getchar().
	if (ch = "y" OR ch = "Y")
	{
		runpath("boot/lib_body_travel_data_return.c").

		if BODY:name <> DEFAULT_KSC
		{
			if (IS_INTER_PLANETARY_MISSION)						// INTER-PLANETARY Mission ?
				RUNPATH( "boot/Phase Angle.c", BODY(DEFAULT_KSC)).	// Warp to the correct: Phase Angle
				
			activateAllEngines().
			if body:atm:height > 0 {
				print"ATM: Launch-Orbit".
				runpath("boot/GOUP.c", true).		// ATM: "Launch-Orbit"
				runpath("boot/GOORBIT.c").			// (Prograde) + (Moons)"Phase0-Normal"
			} else {
				print"NOATM: Launch-Orbit".
				RUNPATH( "boot/PhaseIV-Orbit.c" ).					// Re-Launch to TARGET Orbit
				activateAllEngines(). WAIT 1.
				RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
				activateVesselProbe().
			}
			
			RUNPATH( "boot/PhaseV-Return.c" ).					// Return: Burn to "Kerbin SOI"
		}

		if (IS_INTER_PLANETARY_MISSION) {
			RUNPATH( "boot/PhaseI-Transfer.c", "RETROGRADE" ).	
			RUNPATH( "boot/PhaseII-Break.c", DEFAULT_KSC ). 		
		}

		PRINT "Press [ENTER] to LAND!".set ch to terminal:input:getchar().
		RUNPATH( "boot/PhaseVI-Land.c", true ).		 			// Kerbin Reentry & Touch Down
	} else {
		ResetMission(). // Re-define Target
		reboot.
		LOG  STAGE_1_TYPE+" MISSION END" to LOG.TXT.
	}
}