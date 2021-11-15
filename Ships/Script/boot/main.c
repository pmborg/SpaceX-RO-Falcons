// --------------------------------------------------------------------------------------------
// Filename: main.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is called by main processor to Orchestrate all mission phases.
// 06/Nov/2021
// --------------------------------------------------------------------------------------------
LOG   "START: main.c" to LOG_FILE.

// Reset Engine settings before all, ("migth be a reboot")
set thrust to 0.	
lock throttle to thrust.
WAIT 0.

LOG  "SHIP:LAT: "+SHIP:GEOPOSITION:LAT to LOG_FILE.
LOG  "SHIP:LNG: "+SHIP:GEOPOSITION:LNG to LOG_FILE.

runpath("boot/declare-globals.c").
runpath("boot/atm.c").				//needed for orbit/reboots.

if vehicle_type = "SaturnV"
	LOG "Normal" to normal.txt. //Skip normal/correction

global TakeOffTime to TIME:SECONDS. //(define: Secure for reboots)
global last_value1 to 0.
global Aceleration_value1 to 0.
global lat_correction to 0.
global Relative_Inclination_to_Target to 0.

function change_inclination 
{
		PARAMETER force_now is false.
		
		LOG "MAIN: change_inclination-"+force_now to LOG_FILE.
		
		CLEARSCREEN. print " ". print " ".
		
		// Just reaction-wheels Stability:
		RCS OFF.
		SAS OFF.
		LOCK STEERING TO SHIP:prograde.
		wait 1.

		if KUniverse:ActiveVessel <> SHIP {
			update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
			UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		}
		CLEARSCREEN. print " ". print " ".
		//update_phase_title("Confirm: Change-Inclination?", 0, false).
		runpath( "boot/Phase0-Normal.c", mission_target, force_now).// Correct Normal Before Burn
		LOG "Normal" to normal.txt.
		CLEARSCREEN. print " ". print " ".
}

// --------------------------------------------------------------------------------------------
// ACTION: REFUEL?
// --------------------------------------------------------------------------------------------
if (mission_origin <> DEFAULT_KSC and BODY:NAME <> "Sun") and (status = "LANDED" or status = "SPLASHED")
{
	CLEARSCREEN. print " ". print " ".
	update_phase_title("Confirm: START Refuel?", 0, false).
	PRINT "Confirm: START Refuel? (y/n)". set ch to terminal:input:getchar().
	if (ch = "y" OR ch = "Y") {
		traversePartsForResources("fuel").
		runpath("boot/refuel.c").
	}
}

// --------------------------------------------------------------------------------------------
// ACTION: LAUNCH
// --------------------------------------------------------------------------------------------
if NOT EXISTS("resources.txt") 								// Refuelled already?, SKIP "GO-JOURNEY", GOTO "RETURN-JOURNEY"
{
	if (IS_INTER_PLANETARY_MISSION) and (altitude < alt:radar) and (BODY:NAME = "Earth" or BODY:NAME = "Kerbin")
	{
		RUNPATH( "boot/Phase-Angle.c", mission_target ).	// WARP to Correct Phase Angle
		KUniverse:QUICKSAVETO("1-Phase-Angle done").
	}

	//ACTION: Launch-Orbit --------------------------------------------------------
	if ( (  (BODY:name = DEFAULT_KSC and periapsis < body:atm:height) or
	        (BODY:name <> DEFAULT_KSC and (status = "LANDED" or status = "SPLASHED"))  ) and (not EXISTS("CIRCULARIZE.txt")) ) or (BODY:name = DEFAULT_KSC and vehicle_type = "Space4")
	{
		if periapsis < 0
		{
			if body:atm:height > 0
				RUNPATH( "boot/Launch-Orbit.c", LEOrbit ).	// [1] ATM-Launch: "Launch-Orbit.c" (Have Atmosphere)
			else
				RUNPATH( "boot/PhaseIV-Orbit.c" ).			// NO-ATM-Launch: "PhaseIV-Orbit.c"
			
			KUniverse:QUICKSAVETO("2-Launch-Orbit done").
		}
		
		//ACTION: Launch-Circularize --------------------------------------------------------
		if ((BODY:name = mission_origin) and (apoapsis < LEOrbit or periapsis < body:atm:height) or orbit_type = "GSO" or vehicle_type = "Crew Dragon 2" or vehicle_type = "Space4") and NOT EXISTS("CIRCULARIZE.txt")
		{
			if (vehicle_type <> "SN9-Profile1" and vehicle_sub_type <> "SN16-Profile1" and vehicle_sub_type <> "SN20-Profile") or vehicle_type = "Space4"
			{
				if KUniverse:ActiveVessel <> SHIP {
					update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
					UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
				}
				if body:atm:height > 0
					RUNPATH( "boot/Launch-Circularize.c", LEOrbit ).
				else
					RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
				KUniverse:QUICKSAVETO("3-Launch-Circularize done").
			}
		} else
			LOG  "SKIP: Launch-Circularize" to LOG_FILE.
	} else
		LOG  "SKIP: Launch" to LOG_FILE.

	// --------------------------------------------------------------------------------------------
	// ACTION: ADJUST Normal/Inclination?
	// --------------------------------------------------------------------------------------------
	// Adjust mission inclination?
	if (vehicle_type <> "SN9-Profile1" and vehicle_sub_type <> "SN16-Profile1" and vehicle_sub_type <> "SN20-Profile")
	{
		if NOT EXISTS("normal.txt") {
			LOG "part-1-change_inclination" to LOG_FILE.
			change_inclination().
		}
		else
			LOG  "SKIP: Normal" to LOG_FILE.

		//ACTION: BURN ----------------------------------------------------
		if STATUS = "ORBITING" and apoapsis > body:atm:height and periapsis > body:atm:height and (BODY:name = mission_origin or BODY:name = "Sun" or BODY:name = "Moon")
			LOG  "ACTION: BURN" to LOG_FILE.
			if BODY:name <> mission_target:name 
			{
				CLEARSCREEN. print " ". print " ".
				update_phase_title("BURN MAIN STAGE", 0, false).
				//SaturnV:
				if vehicle_type = "SaturnV" and mass > 120 { stage. wait 2. }
				if vehicle_type = "SaturnV" and mass > 120 { stage. wait 2. }
				if vehicle_type = "SaturnV" and mass > 120 { stage. wait 2. }

				runpath("boot/BURN.c"). // BEFORE BREAK&LAND
			}
		else
			LOG  "SKIP: BURN" to LOG_FILE.
	}
	
	// --------------------------------------------------------------------------------------------
	// ACTION: LEM/DOCK?
	// --------------------------------------------------------------------------------------------
	LOG  "LEM/DOCK?" to LOG_FILE.
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
			PRINT "SWITCH TO LEM AND WAIT...".
			PRINT "Press [ENTER], to Confirm: EARTH RETURN".
			set ch to terminal:input:getchar().
		}
		else
			LOG  "SKIP: LEM/DOCK" to LOG_FILE.
	}

	LOG  "ON TARGET -> ACTION: Break & LAND!" to LOG_FILE.
	// ACTION: Break & LAND! -------------------------------------------
	if vehicle_type = "Space4" or vehicle_type = "SaturnV"
	{
		CLEARSCREEN. print " ". print " ".
		update_phase_title("ACTION: Break & LAND", 1, false).
		if NOT EXISTS("orbit_target.txt")
		{
			//Decrease orbit to PE: 150km
			update_phase_title("Process Low Orbit", 1, false).
			retrograde_check().
			set thrust to 1.
			RCS OFF.
			until (periapsis <= 5000*1000) WAIT 0.1.
			set thrust to 0.10.
			until (periapsis <= 150*1000) WAIT 0.01.
			set thrust to 0.

			//Do Circular orbit for PE: 300km
			warp_until_periapsis(60).
			SAS ON.
			set sasmode TO "retrograde". wait 1.
			set thrust to 1.
			RCS OFF.
			until (apoapsis <= 150*1000) WAIT 0.01.
			set thrust to 0.

			LOG "Done" to orbit_target.txt.
		}
		 
		 //Process target landing:
		 if periapsis > 100000
			warp_until_periapsis(30).
		 RUNPATH( "boot/PhaseIII-Land.c" ).  	// Auto-Land / Touch-Down
		 
		 
	} else if (vehicle_type <> "SN9-Profile1" and vehicle_sub_type <> "SN16-Profile1" and vehicle_sub_type <> "SN20-Profile" and vehicle_sub_type <> "StarShip")
	{
		if status <> "LANDED" and status <> "SPLASHED"
		{
			if NOT EXISTS("CIRCULARIZE.txt")
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
				SET MAPVIEW TO FALSE. wait 0.1. set sasmode TO "RADIALIN". wait 1.
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
	} else {
		RUNPATH( "boot/starship_lowentry_return.c").
	}
	
	print " ". print " ".
	PRINT "Press [ENTER], to Confirm, REBOOT!" at (0,35). set ch to terminal:input:getchar().
	reboot.
}

// RETURN TO EARTH / INBOUND ---------------------------------------------------------
if BODY:name <> DEFAULT_KSC {
	CLEARSCREEN. print " ". print " ".
	PRINT "Press [y/n] y to Confirm the Kerbin Return, n to new target!".set ch to terminal:input:getchar().
	if (ch = "y" OR ch = "Y")
	{
		runpath("boot/lib_body_travel_data_return.c").

		if BODY:name <> DEFAULT_KSC
		{
			if (IS_INTER_PLANETARY_MISSION)							// INTER-PLANETARY Mission ?
				RUNPATH( "boot/Phase Angle.c", BODY(DEFAULT_KSC)).	// Warp to the correct: Phase Angle
				
			activateAllEngines().
			if body:atm:height > 0 {
				print"ATM: Launch-Orbit".
				runpath("boot/GOUP.c", true).						// ATM: "Launch-Orbit"
				runpath("boot/GOORBIT.c").							// (Prograde) + (Moons)"Phase0-Normal"
			} else {
				print"NOATM: Launch-Orbit".
				RUNPATH( "boot/PhaseIV-Orbit.c" ).					// Re-Launch to TARGET Orbit
				activateAllEngines(). WAIT 1.
				RUNPATH( "boot/Launch-Circularize.c", apoapsis ).
				activateVesselProbe().
			}
			
			RUNPATH( "boot/PhaseV-Return.c" ).						// Return: Burn to "Kerbin SOI"
		}

		if (IS_INTER_PLANETARY_MISSION) {
			RUNPATH( "boot/PhaseI-Transfer.c", "RETROGRADE" ).	
			RUNPATH( "boot/PhaseII-Break.c", DEFAULT_KSC ). 		
		}

		PRINT "Press [ENTER] to LAND!".set ch to terminal:input:getchar().
		RUNPATH( "boot/PhaseVI-Land.c", true ).		 				// Kerbin Reentry & Touch Down
	} else {
		LOG  STAGE_1_TYPE+" MISSION ABORTED" to LOG_FILE.
		PRINT "Press [ENTER] to ResetMission".set ch to terminal:input:getchar().
		ResetMission(). // Re-define Target
		reboot.
	}
}

LOG   "END: main.c" to LOG_FILE.
LOG   "-----------------" to LOG_FILE.