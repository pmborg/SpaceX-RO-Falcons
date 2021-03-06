// --------------------------------------------------------------------------------------------
// Filename: main.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is called by main processor to Orchestre all mission phases.
// 04/Jan/2021
// --------------------------------------------------------------------------------------------

// Reset Engine settings before all, ("migth be a reboot")
set thrust to 0.	
lock throttle to thrust.
WAIT 0.

runpath("boot/declare-globals.c").
runpath("boot/atm.c").

if vehicle_type = "SaturnV"
	LOG "Normal" to normal.txt. //Skip normal/correction

function change_inclination 
{
		//parameter mission_target.
		
		CLEARSCREEN. print " ". print " ".
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
		CLEARSCREEN. print " ". print " ".
		update_phase_title("Confirm: Change-Inclination?", 0, false).
		//PRINT "Please make sure that TARGET is selected".
		runpath( "boot/Phase0-Normal.c", mission_target).	// Correct Normal Before Burn
		LOG "Normal" to normal.txt.
		CLEARSCREEN. print " ". print " ".
}

//ACTION: REFUEL ----------------------------------------------
if mission_origin <> DEFAULT_KSC //mission_target //ORIGIN = TARGET
{
	CLEARSCREEN. print " ". print " ".
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
	      (BODY:name <> DEFAULT_KSC and (status = "LANDED" or status = "SPLASHED"))  ) and (not EXISTS("CIRCULARIZE.txt"))
	{
		if periapsis < 0
		{
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Orbit.c", LEOrbit ).	// [1] ATM-Launch: "Launch-Orbit.c" (Have Atmosphere)
			else
				RUNPATH( "boot/PhaseIV-Orbit.c" ).			// NO-ATM-Launch: "PhaseIV-Orbit.c"
		}
		
		if (BODY:name = mission_origin) and (apoapsis < LEOrbit or periapsis < body:atm:height) or orbit_type = "GSO" or vehicle_type = "Crew Dragon 2"
		{
			if KUniverse:ActiveVessel <> SHIP {
				update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
				UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
			}
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).
			else
				RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
		} else
			LOG  "SKIP: Launch-Circularize" to LOG.txt.
	} else
		LOG  "SKIP: Launch" to LOG.txt.

	// Adjust mission inclination?
	if NOT EXISTS("normal.txt")
		change_inclination().
	else
		LOG  "SKIP: Normal" to LOG.txt.

	//ACTION: BURN ----------------------------------------------------
	if STATUS = "ORBITING" and apoapsis > body:atm:height and periapsis > body:atm:height and BODY:name = mission_origin
		if BODY:name <> mission_target:name 
		{
			CLEARSCREEN. print " ". print " ".
			update_phase_title("MAIN STAGE", 0, false).
			if vehicle_type = "SaturnV" and mass > 120 { stage. wait 2. }
			if vehicle_type = "SaturnV" and mass > 120 { stage. wait 2. }
			if vehicle_type = "SaturnV" and mass > 120 { stage. wait 2. }

			runpath("boot/BURN.c").
		}
	else
		LOG  "SKIP: BURN" to LOG.txt.

	if BODY:name = mission_target:name and not EXISTS("lem.txt")
	{
		if vehicle_type = "SaturnV"
		{
			clearscreen. print " ". print " ".
			update_phase_title(BODY+" CAPTURE", 1, true).
			SET MAPVIEW TO TRUE.
			SAS OFF. wait 0.1.
			lock steering to retrograde. wait 0.1.
			RCS OFF. wait 0.1.
			lock throttle to thrust.
			set thrust to 1. wait 0.1.
			update_phase_title("wait for 200km", 1, true).
			WAIT UNTIL periapsis < 200000.
			set thrust to 0.05.
			update_phase_title("wait for 114km", 1, true).
			WAIT UNTIL periapsis < 114000. //Moon Orbit PE: 114km (Ideal to do the LM landing + Ideal to do a cheaper re-orbit) 
		
			set thrust to 0.
			SET MAPVIEW TO FALSE.
			AG9 OFF. wait 1.
			AG9 ON. wait 1.

			update_phase_title("MOVE CREW TO LEM", 1, true).
			WAIT UNTIL mass < 25. //WAIT FOR UNDOCK
			
			RCS ON.
			set ShipDockingPort to LEMPortGetter(SHIP, "none").				//GET SM PORT
			set target_vessel to getNearbyProbe(0, "Ship", 100).			//GET LEM
			set TargetDockingPort to LEMPortGetter(target_vessel, "none").	//GET LEM PORT
			update_phase_title("UN-DOCKING 5m", 0, true).
			ApproachDockingPort(ShipDockingPort, TargetDockingPort, 20, 1).
			RCS OFF. wait 1.
			SET KUniverse:ACTIVEVESSEL TO VESSEL(target_vessel:NAME).
			
			clearscreen. print " ". print " ".
			PRINT "SWTICH TO LEM AND WAIT...".
			PRINT "Press [ENTER], to Confirm: EARTH RETURN".
			set ch to terminal:input:getchar().
		}
	}

	//ACTION: Break & LAND! -------------------------------------------
	if status <> "LANDED" and status <> "SPLASHED"
	{
		if EXISTS("CIRCULARIZE.txt")
			RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).

		if KUniverse:ActiveVessel <> SHIP {
			update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
			UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		}
		CLEARSCREEN. print " ". print " ".
		RCS OFF.
		SAS OFF.
		LOCK STEERING TO SHIP:prograde.
		wait 1.
		update_phase_title("Mission Command", 0, false).
		PRINT "Press: 1 - Abort/Land Anywhere!". 
		PRINT "Press: 2 - Circularize(or Stablize ST-2)". 
		if vehicle_type <> "Crew Dragon 2"
			PRINT "Press: 3 - Stage: Customer Payload". 
		else
			PRINT "Press: 4 - Deorbit". 
		PRINT "Press: 5 - Change Inclination".
		set ch to terminal:input:getchar(). PRINT "selected: "+ch.
		if ch="1" or ch =""  {
			CLEARSCREEN. print " ". print " ".
			update_phase_title("Confirm: SPEED-BREAK?", 0, false).
			PRINT "Confirm: SPEED-BREAK? (y/n)". set ch to terminal:input:getchar().
			if (ch = "y" OR ch = "Y")
				RUNPATH( "boot/PhaseII-Break.c", mission_target ).	 // Burn Capture & Burn Circularize: [NOW5]
			
			RUNPATH( "boot/PhaseIII-Land.c" ).  	// Auto-Land / Touch-Down
		}
		else if ch="2" {
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).
			else
				RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
			reboot.
		}
		else if ch="3" {
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
			CLEARSCREEN. print " ". print " ".
		}
		else if ch="4" {
			runpath("boot/stage-2-deorbit.c").
		}
		else if ch="5" {
			if vehicle_type = "Crew Dragon 2" 
				change_inclination (VESSEL("PMB ISS HT2 & TANTARES KSP1.10")).
		}
	}
	
	LOG  STAGE_1_TYPE+" REBOOT FOR RE-FUEL" to LOG.TXT.
}

//INBOUND ---------------------------------------------------------
if BODY:name <> DEFAULT_KSC {
	CLEARSCREEN. print " ". print " ".
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