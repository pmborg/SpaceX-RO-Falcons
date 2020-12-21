// --------------------------------------------------------------------------------------------
// Filename: useful-tools.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				General functions used by other mission files.
// 13/Dez/2020
// --------------------------------------------------------------------------------------------
set phase_title_position to 0.

function steering_falcon
{
	parameter Vdeg.	//SET Vdeg to 90-delta.						// Vertical = 90
	
	set lat_correction to 0.
	if KUniverse:ActiveVessel = SHIP
		if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT and vehicle_sub_type <> "Falcon Heavy LEM"
	set lat_correction to (VESSEL("Landingzone1"):GEOPOSITION:LAT - ADDONS:TR:IMPACTPOS:LAT)*50.

	PRINT "[GEOPOSITION]: " at (0,17).
	PRINT ROUND (GEOPOSITION:LAT,3)+", " +ROUND (GEOPOSITION:LNG,3)+"   " at (22,17).
	PRINT "lat.correction: " at (0,18). PRINT ROUND(lat_correction,2) at (22,18).
	SET steeringDir TO (-90-lat_correction).	// W/E
	set Vroll to -270.							// -270 = Zero Rotation
	
	LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).
}

function st1_stage 
{
if SHIP_NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5" and STAGE:NUMBER <= 4
or SHIP_NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5 Crew Dragon 2" and STAGE:NUMBER <= 8
or SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5" and STAGE:NUMBER <= 4
	return.

	// ST-1 SEPARATION
	UNTIL SHIP:MAXTHRUST <= 0
	{
		UNLOCK STEERING.
		SAS OFF.
		wait 0.5.
		if (KUniverse:ActiveVessel = SHIP) STAGE.
	}

	// ST-2 Ignite the engine (WAIT for thrust)
	UNTIL SHIP:MAXTHRUST > 0
	{
		if (KUniverse:ActiveVessel = SHIP) STAGE.
		SAS ON.
		wait 0.25.
		set sasmode TO "PROGRADE".
		wait 0.25.
		set thrust to 0.25.
		wait 0.25.
		PRINT "Stage Nº: "+ stage:number.

		// Backup plan: to Start engine, due failor of vapors in fuel line:
		if SHIP:MAXTHRUST <= 0 and stage:number = 1
		{
			shutDownAllEngines().
			activateAllEngines().
		}
	}

	// WAIT in Prograde before heading correction:
	if vehicle_type = "Crew Dragon 2"
		WAIT 1.
	else
		WAIT 1.75.
}

function confirm_stage 
{
	IF (KUniverse:ActiveVessel <> SHIP) 
		do_stage().
	else {
		PRINT "Confirm Stage? (y/n)". set ch to terminal:input:getchar().
		if (ch = "y" OR ch = "Y") 
			stage.
	}
}

function shutDownAllEngines 
{
	LIST ENGINES IN enginesList.
	FOR eng IN enginesList {
		if eng:typename() = "Engine"
			eng:SHUTDOWN.
	}.
}

function activateOneEngine 
{
	shutDownAllEngines().
	LIST ENGINES IN enginesList.
	set n to 0.
	FOR eng IN enginesList {
		if eng:typename() = "Engine" and n < 1
			eng:ACTIVATE.
		if eng:POSSIBLETHRUST > 710
			set n to n+1.
	}.
}

function activateAllEngines 
{
	LIST ENGINES IN enginesList.
	FOR eng IN enginesList {
		if eng:typename() = "Engine"
			eng:ACTIVATE.
	}.
}

function activate3engines
{
	shutDownAllEngines().
	AG2 OFF. 	//Reset 3 main engines
	wait 1.
	AG2 ON. 	//Activate 3 main engines
	wait 1.
}

function engines_thrustlimit_to 
{
	parameter LIMIT.
	
	LIST ENGINES IN myVariable.
	FOR eng IN myVariable {
		set eng:THRUSTLIMIT to LIMIT.
	}.
}

function update_phase_title
{
	parameter phase.
	parameter play_sound.
	parameter stack_phases is true.
	
	PRINT phase+"          " at (0,0). 
	if (stack_phases)
	{
		set y to 16.
		PRINT "   " at (25,y+phase_title_position). 
		PRINT "-->"+phase at (25,y+1+phase_title_position). 
		set phase_title_position to phase_title_position+1.
	}
	//ORI
	PRINT "--------------------------------------------------"at (0,1). 
	
	if STAGE_1_TYPE = "MASTER"		//FH
		PRINT "(Mast)" at (44,1).
	else 
	if STAGE_1_TYPE = "SLAVE"		//FH
		PRINT "(Slav)" at (44,1).
	else 
	if STAGE_1_TYPE = "CORE" 		//FH or ST1
		PRINT "(Core)" at (44,1).
	else 
	if STAGE_1_TYPE = "ST-1" 		//FH
		PRINT "(ST-1)" at (44,1).
	else 
	if STAGE_1_TYPE = "ST-2" 		
		PRINT "(ST-2)" at (44,1).
	else 
	if STAGE_1_TYPE = "MAIN" 		//MAIN CPU
		PRINT "(MAIN)" at (44,1).
		
	if status <> "PRELAUNCH" and play_sound = 1
	{
		SET V0 TO GETVOICE(0). 		// Gets a reference to the zero-th voice in the chip.
		V0:PLAY( NOTE(125, 0.1) ).  // Starts a note at 100 Hz for 0.1 seconds.
	}
	WAIT 0.5.
	PRINT "| Profile: "+vehicle_type + " " at (24,0). //avoid overlap (KOS Bug) here, instead in ORI.
	
	LOG time:clock +": " + STAGE_1_TYPE + " - PHASE: "+phase to LOG.txt.
}

function updateVars  //Scalar projection of two vectors. Find component of a along b. a(dot)b/||b||
{
	//set g TO constant:G * BODY:Mass / BODY:RADIUS^2.
	set r to altitude+BODY:radius. 	//600000 (KERBIN)
	set GM to BODY:mu. 				//GM = 3.5316*(10^12). (KERBIN)
	set g to GM/(r^2).				//Calculate g = 9.81	
}

function log_data
{
	parameter Vs2.
	parameter showlog to false.	//true = TURN ON LOG file.
	
	updateVars().
	
	if (not showlog)
		return.
	
	PRINT "TIME,   VELO,   ALT,    Acel,   Q     --LOG:FILE" at (0,34).
	
	set value1 to ROUND((TIME:SECONDS-TakeOffTime),1).
	PRINT value1+" " at (0,35).
	
	set value2 to ROUND(Vs2,0).
	PRINT value2+" " at (8,35).

	set value3 to ROUND((SHIP:altitude-145)/1000,3).
	PRINT value3+" " at (8*2,35).

	set Ac to SHIP:SENSORS:ACC.
	set Acx to Ac:x.
	set Acy to Ac:y.
	set Acz to Ac:z.	
	
	set Aceleration_value1 to ROUND(g+SQRT((Acx^2)+(Acy^2)+(Acz^2)),1).
	PRINT Aceleration_value1+" " at (8*3,35).
	
	//v1
	// set q to ROUND(SHIP:DYNAMICPRESSURE).
	//v2
	// set H to altitude/(-5000).
	// set p to p0*(e^H).
	// set q to ROUND(.5*p*(Vs2^2),1). // pd = 1/2 ρ v^2
	//v3
	if ROUND(BODY:ATM:ALTITUDEPRESSURE(h),4) = 0 and ROUND(BODY:ATM:ALTITUDETEMPERATURE(h),1) = 0
		PRINT "--    " at (8*4,35).
	else
		PRINT ROUND(qVal,0)+" " at (8*4,35).
		
	if (ROUND (value1,0) > last_value1) 
	{
		set last_value1 to ROUND (value1,0).
		LOG value1+","+value2+","+value3+","+Aceleration_value1+","+ROUND(qVal,0) to FLIGHT_LOG.txt.
	}
	PRINT "T.ORB: "+orbit_type at (8*5-2,35).
}

function east_for 
{
  parameter ves to SHIP.

  return vcrs(ves:up:vector, ves:north:vector).
}

function vessel_compass
{
  parameter ves to SHIP.

  local pointing is ves:facing:forevector.
  local east is east_for(ves).

  local trig_x is vdot(ves:north:vector, pointing).
  local trig_y is vdot(east, pointing).

  local result is arctan2(trig_y, trig_x).

  if result < 0 {
	return 360 + result.
  } else {
	return result.
  }
}

function vessel_pitch 
{
  parameter ves to SHIP.

  return 90 - vang(ves:up:vector, ves:facing:forevector).
}

////////////////////////////////////////////////////////////////////////////////////////////////
function flip_maneuver
{
	SAS OFF.
	RCS ON. //OFF.

	if STAGE_1_TYPE = "MASTER" or STAGE_1_TYPE = "SLAVE"
	{
		update_phase_title("HEAVY-1 SEPARATION...   ", 0, true).
		SET thrust TO 0.2.
		lock throttle to thrust.
		SET Vdeg to shipPitch.
		SET steeringDir TO 90.	// W/E
		set Vroll to -270.		// -270 = Zero Rotation
		
		if STAGE_1_TYPE = "MASTER"
			set steeringDir TO steeringDir+45.
		if STAGE_1_TYPE = "SLAVE"
			set steeringDir TO steeringDir-45.
			
		LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).//steering_falcon(Vdeg).
		//WAIT 5. // wait for SEP
	} else {
		update_phase_title("STAGE-1 SEPARATION...   ", 0, true).
		LOCK STEERING TO SHIP:PROGRADE  + R(0,0,180).
		//WAIT 7. // wait for SEP
	}
	
	WAIT 5. // wait for SEP
	PRINT_STATUS (3).
	
	CLEARSCREEN.
	RCS ON.
	update_phase_title("FLIP MANEUVER   ", 0, true).
	set wait_max_sec to 25.
	FROM {local x is wait_max_sec.} UNTIL x = 0 STEP {set x to x-1.} DO 
	{
		print "wait: "+x+" " at (42,2).
		
		if STAGE_1_TYPE = "CORE"
			LOCK STEERING TO HEADING(270,0, -270).						// For Drone Ship Landing
		else
		{
			steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset). // Heading to Return Home
			PRINT_STATUS (3).
		}
		
		PRINT_STATUS (3).
		if shipPitch>-5 and shipPitch<5 and x < (wait_max_sec-5)
			set x to 1.	//End the Wait Cicle...
		
		WAIT 1. //Mandatory wait.
	}
	print "wait: --" at (42,2).
}
