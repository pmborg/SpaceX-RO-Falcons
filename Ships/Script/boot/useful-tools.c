// --------------------------------------------------------------------------------------------
// Filename: useful-tools.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				General functions used by other mission files.
// 04/Nov/2021
// --------------------------------------------------------------------------------------------
set phase_title_position to 0.

//for falcons & SS
function steering_falcon
{
	parameter Vdeg.	//SET Vdeg to 90-delta.						// Vertical = 90
	parameter lat_correction is 0.
	parameter roll is -270.
	
	if vehicle_company = "SpaceX" and not splash_landing
	{
		if KUniverse:ActiveVessel = SHIP
			if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT and vehicle_sub_type <> "Falcon Heavy LEM"
		set lat_correction to (VESSEL("Landingzone1"):GEOPOSITION:LAT - ADDONS:TR:IMPACTPOS:LAT)*50.
	}

	set steeringDir TO (-90-lat_correction).	// W/E
	set steeringVdeg to Vdeg.
	set steeringVroll to roll.					// -270 = Zero Rotation
	//LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll). (NO NEED THIS LINE AUTOMATIC!)
}

//for booster:
function st1_stage 
{
if SHIP_NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5" and STAGE:NUMBER <= 4
or SHIP_NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5 Crew Dragon 2" and STAGE:NUMBER <= 8
or SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5" and STAGE:NUMBER <= 4
	return.

	// ST-1 SEPARATION
	UNTIL SHIP:MAXTHRUST <= 0
	{
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
		SET MAPVIEW TO FALSE. wait 2. set sasmode TO "PROGRADE".
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
	}
	LOG "shutDownAllEngines ()" to LOG_FILE.
}

//for falcons:
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
	}
	LOG "activateOneEngine ()" to LOG_FILE.
}

function activateLEMengine 
{
	shutDownAllEngines().
	LIST ENGINES IN enginesList.
	FOR eng IN enginesList {
		if eng:POSSIBLETHRUST > 40
			eng:ACTIVATE.
	}
	LOG "activateLEMengine ()" to LOG_FILE.
}

function activateAllEngines 
{
	LIST ENGINES IN enginesList.
	FOR eng IN enginesList {
		if eng:typename() = "Engine"
			eng:ACTIVATE.
	}
	LOG "activateAllEngines ()" to LOG_FILE.
}

//for falcons:
function activate3engines
{
	shutDownAllEngines().
	AG2 OFF. wait 1.	//Reset 3 main engines
	AG2 ON.  wait 1.	//Activate 3 main engines
	LOG "activate3engines ()" to LOG_FILE.
}

//for falcons:
function engines_thrustlimit_to 
{
	parameter LIMIT.
	
	LIST ENGINES IN myVariable.
	FOR eng IN myVariable {
		set eng:THRUSTLIMIT to LIMIT.
	}
	LOG "engines_thrustlimit_to ()" to LOG_FILE.
}

//for SS:
function sn11_test_profile_deactivate_engine1
{
	AG3 OFF. wait 0.1.
	AG3 ON.  wait 0.1.
}

//for SS:
function sn11_test_profile_deactivate_engine2
{
	AG4 OFF. wait 0.1.
	AG4 ON.  wait 0.1.
}


function update_phase_title
{
	parameter phase is "".					//Title
	parameter play_sound is 0.				//0 or 1
	parameter stack_phases is true.		//true = STACK
	parameter y is 16.
	parameter x is 25.
	
	PRINT phase+"          " at (0,0). 
	if (stack_phases)
	{
		PRINT "   " at (x,y+phase_title_position). 
		PRINT "-->"+phase at (x,y+1+phase_title_position). 
		set phase_title_position to phase_title_position+1.
	}
	//ORI
	PRINT "--------------------------------------------------"at (0,1). 
	if kspver > 0
		PRINT "ksp "+kspver at (20,1). 
	
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
	if STAGE_1_TYPE = "ST-3"
		PRINT "(ST-3)" at (44,1).
	else 
	if STAGE_1_TYPE = "LEM"
		PRINT "LEM" at (44,1).
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
	
	LOG "#"+time:clock +": " + STAGE_1_TYPE + " - PHASE: "+phase to LOG_FILE.
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
	
	if vehicle_company = "SpaceX"
		set showlog to true.
	
	updateVars().
	
	if (not showlog)
		return.
	
	PRINT "TIME,    VELO,   ALT,    Acel,   Q     --LOG:FILE" at (0,34).
	
	set value1 to ROUND((TIME:SECONDS-TakeOffTime),1).
	PRINT "T+"+value1+" " at (0,35).
	
	set value2 to ROUND(Vs2,0).
	PRINT value2+" " at (1+8,35).

	set value3 to ROUND((SHIP:altitude-145)/1000,3).
	PRINT value3+" " at (1+8*2,35).

	set Ac to SHIP:SENSORS:ACC.
	set Acx to Ac:x.
	set Acy to Ac:y.
	set Acz to Ac:z.	
	
	set Aceleration_value1 to ROUND(g+SQRT((Acx^2)+(Acy^2)+(Acz^2)),1).
	PRINT Aceleration_value1+"   " at (1+8*3,35).
	
	//v1
	// set q to ROUND(SHIP:DYNAMICPRESSURE).
	//v2
	// set H to altitude/(-5000).
	// set p to p0*(e^H).
	// set q to ROUND(.5*p*(Vs2^2),1). // pd = 1/2 ρ v^2
	//v3
	if ROUND(BODY:ATM:ALTITUDEPRESSURE(altitude),4) = 0 and ROUND(BODY:ATM:ALTITUDETEMPERATURE(altitude),1) = 0
		PRINT "--    " at (1+8*4,35).
	else
		PRINT ROUND(qVal/1000,0)+" " at (1+8*4,35).
		
	if (ROUND (value1,0) > last_value1) 
	{
		set last_value1 to ROUND (value1,0).
		LOG value1+","+value2+","+value3+","+Aceleration_value1+","+ROUND(qVal/1000,0) to FLIGHT_LOG_FILE.
	}
	PRINT "T.ORB: "+orbit_type at (1+8*5-2,35).
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

// FLIP --------------------------------------------------------------------------------------------
function flip_maneuver
{
	LOG "flip_maneuver" to LOG_FILE.
	
	SAS OFF.
	RCS ON. //OFF.

	//FH PRE FLIP MANEUVER
	if STAGE_1_TYPE = "MASTER" or STAGE_1_TYPE = "SLAVE"
	{
		update_phase_title("HEAVY-1 SEPARATION...", 0, true).
		SET thrust TO 0.2.
		lock throttle to thrust.
		SET steeringVdeg to shipPitch.
		SET steeringDir TO 90.	// W/E
		set steeringVroll to -270.		// -270 = Zero Rotation
		
		if STAGE_1_TYPE = "MASTER"
			set steeringDir TO steeringDir+45.
		if STAGE_1_TYPE = "SLAVE"
			set steeringDir TO steeringDir-45.
			
		LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).//steering_falcon(Vdeg).
	} else {
		update_phase_title("STAGE-1 SEPARATION...", 0, true).
		LOCK STEERING TO SHIP:PROGRADE  + R(0,0,180).
	}
	
	if vehicle_type = "Crew Dragon 2"
		WAIT 1.
	else
		WAIT 5. // wait for SEP
	PRINT_STATUS (3).
	
	//NORMAL FLIP MANEUVER -------------------------------------------------------
	CLEARSCREEN.
	update_phase_title("FLIP MANEUVER", 0, true).
	RCS ON.
	if vehicle_type = "StarShip"
		set wait_max_sec to 30.
	else
		set wait_max_sec to 25.
	
	if STAGE_1_TYPE = "CORE"
		LOCK STEERING TO HEADING(270,0, -270).						// For Drone Ship Landing
	FROM {local x is wait_max_sec.} UNTIL x = 0 STEP {set x to x-1.} DO 
	{
		print "wait: "+x+" " at (41,2).
		
		if STAGE_1_TYPE <> "CORE"
			steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset). // Heading to Return Home
		
		PRINT_STATUS (3).
		if shipPitch>-5 and shipPitch<5 and x < (wait_max_sec-5)
			set x to 1.	//End the Wait Cicle...
		
		WAIT 1. //Mandatory wait.
	}
	print "wait: --" at (42,2).
}

	function update_steering_status
	{
		//DEBUG:
		print ROUND (steeringDir) at   (40,3).
		print ROUND (steeringVdeg) at  (40,4).
		print ROUND (steeringVroll) at (40,5).
	}


// DOCKING --------------------------------------------------------------------------------------------
Function Translate {
  Parameter SomeVector.
  if SomeVector:mag > 1 {
    set SomeVector to SomeVector:normalized.
  }

  set ship:control:starboard to SomeVector * ship:facing:starvector.
  set ship:control:fore      to SomeVector * ship:facing:forevector.
  set ship:control:top       to SomeVector * ship:facing:topvector.
}

Function KillRelVelRCS {
  Parameter target_vessel.

  local lock RelativeVelocity to (ship:velocity:orbit - target_vessel:velocity:orbit).
  until RelativeVelocity:mag < 0.1 {
    Translate(-1*RelativeVelocity).
  }
  Translate(V(0,0,0)).
}

Function LEMPortGetter {
  Parameter NameOfVessel is ship.
  Parameter PrePickedPort is "none".
  if PrePickedPort = "none" {

    local portlist is list().
    for port in NameOfVessel:partsdubbedpattern("dock"){
		if port:name:FIND("FASAApollo.SM.Light") < 0
			portlist:add(port).
		log port to LOG_FILE.
    }

    local PortNumber is 0.
    until false {
        return portlist[PortNumber].
    }
  } else {
    return PrePickedPort.
  }
}

Function PortGetter {
  Parameter NameOfVessel is ship.
  Parameter PrePickedPort is "none".
  if PrePickedPort = "none" {

    local portlist is list().
    for port in NameOfVessel:partsdubbedpattern("dock"){
		portlist:add(port).
    }

    local PortNumber is 0.
    until false {
      if portlist[PortNumber]:state = "ready" {
        return portlist[PortNumber].
      } else {
        set PortNumber to PortNumber + 1.
        if PortNumber = PortList:length {
          print "ERROR NO PORTS READY TO BE USED".
        }
      }
    }
  } else {
    return PrePickedPort.
  }
}

Function ApproachDockingPort {
  Parameter ShipDockingPort.
  Parameter TargetDockingPort.
  Parameter Distance.
  Parameter Speed.
  Parameter ErrorAllowed is 0.2.

  ShipDockingPort:controlfrom.

  local Lock DistanceInFrontOfPort to TargetDockingPort:portfacing:vector:normalized * Distance.
  local Lock ShipToDIFOP to TargetDockingPort:nodeposition - ShipDockingPort:nodeposition + DistanceInFrontOfPort.
  local Lock RelativeVelocity to ship:velocity:orbit - TargetDockingPort:ship:velocity:orbit.

  until FALSE
  {
    Translate((ShipToDIFOP:normalized*Speed) - RelativeVelocity).
	
	//DEBUG:
	clearvecdraws().
    vecdraw(TargetDockingPort:position, DistanceInFrontOfPort, RGB(1,0,0), "DistanceInFrontOfPort", 1.0, true, 0.2).
    vecdraw(v(0,0,0), ShipToDIFOP, RGB(0,1,0), "ShipToDIFOP", 1.0, true, 0.2).
	
    local DistanceVector is (TargetDockingPort:nodeposition - ShipDockingPort:nodeposition).
	set error to abs(Distance - DistanceVector:mag).
	print "Distance Error: "+ROUND(error,2)+"    " at (0,15).
	set ang to vang(ShipDockingPort:portfacing:vector, DistanceVector).
	print "Current Angle:  "+ROUND(ang,2)+"    " at (0,16).
	if ang < 30 and SAS //Check if we can use SAS
	{
		SAS ON. wait 0.01.
		if (SAS )
		{
			SET MAPVIEW TO FALSE.
			set sasmode to "TARGET". wait 0.01.
		}
	}
    if (ang < 2) and (error < ErrorAllowed)
      break.
  }
  Translate(v(0,0,0)).
  //SAS OFF.
}

// Percistant variable:
// set angle to 0.

//----------------------------------------------------------------------------------
//RelativeAngleCalculation: between to planes
function getNormalOrbitAngle 
{
	set a1 to sin(ship:Orbit:Inclination)*cos(ship:orbit:LAN).
	set a2 to sin(ship:Orbit:Inclination)*sin(ship:orbit:LAN).
	set a3 to cos(ship:Orbit:Inclination).

	set b1 to sin(target:Orbit:Inclination)*cos(target:orbit:LAN).
	set b2 to sin(target:Orbit:Inclination)*sin(target:orbit:LAN).
	set b3 to cos(target:Orbit:Inclination).

	set res to a1*b1+a2*b2+a3*b3.
	// if res > 180 or res < 0
		// LOG "MATH ERROR - res: "+res to LOG_FILE.	//skip this iteration and use last one.
	// else
		set angle to arccos(res).
	
	return angle.
}

//SOURCE INFO: https://ksp-kos.github.io/KOS/tutorials/exenode.html
function execute_node 
{
PARAMETER nd.
PARAMETER wait_for_node is true.
PARAMETER  point_ship_to_maneuver is true.
	
	//Print out node's - ETA and deltaV
	print "Node in: " + round(nd:eta) + ", DeltaV: " + round(nd:deltav:mag).	

	//Calculate ship's max acceleration:
	set max_acc to ship:maxthrust/ship:mass.
	
	//Calculate burn duration:
	if max_acc <= 0 {
		print "No engine POWER!".
		return false.
	}
	set burn_duration to nd:deltav:mag/max_acc.
	print "Crude Estimated burn duration: " + round(burn_duration) + "s".	
	
	if wait_for_node
		wait until nd:eta <= (burn_duration/2 + 60).

	set np to nd:deltav. //points to node, don't care about the roll direction.
	unlock steering.
	SET MAPVIEW TO FALSE. wait 2. set sasmode to "maneuver". wait 0.1.
		
	if point_ship_to_maneuver {
		//now we need to wait until the burn vector and ship's facing are aligned
		wait until vang(np, ship:facing:vector) < 0.25.

		//the ship is facing the right direction, let's wait for our burn time
		wait until nd:eta <= (burn_duration/2).
	}
	
	//we only need to lock throttle once to a certain variable in the beginning of the loop, and adjust only the variable itself inside it
	set tset to 0.
	lock throttle to tset.


	print "Rel. angle to target: " at (0, 10).
	
	set done to False.
	//initial deltav
	set dv0 to nd:deltav.
	until done
	{
		//recalculate current max_acceleration, as it changes while we burn through fuel
		set max_acc to ship:maxthrust/ship:mass.

		//throttle is 100% until there is less than 1 second of time left to burn
		//when there is less than 1 second - decrease the throttle linearly
		set tset to min(nd:deltav:mag/max_acc, 1).

		//here's the tricky part, we need to cut the throttle as soon as our nd:deltav and initial deltav start facing opposite directions
		//this check is done via checking the dot product of those 2 vectors
		if vdot(dv0, nd:deltav) < 0
		{
			print "End burn, remain dv " + round(nd:deltav:mag,1) + "m/s, vdot: " + round(vdot(dv0, nd:deltav),1).
			lock throttle to 0.
			break.
		}

		//we have very little left to burn, less then 0.1m/s
		if nd:deltav:mag < 0.1
		{
			print "Finalizing burn, remain dv " + round(nd:deltav:mag,1) + "m/s, vdot: " + round(vdot(dv0, nd:deltav),1).
			//we burn slowly until our node vector starts to drift significantly from initial vector
			//this usually means we are on point
			wait until vdot(dv0, nd:deltav) < 0.5.

			lock throttle to 0.
			print "End burn, remain dv " + round(nd:deltav:mag,1) + "m/s, vdot: " + round(vdot(dv0, nd:deltav),1).
			set done to True.
		}
		
		if nd:deltav:mag > 50
			print ROUND (getNormalOrbitAngle(),2) at (25, 10).
			
		wait 0.1.
	}
	unlock steering.
	unlock throttle.
	wait 1.	
	

	//we no longer need the maneuver node
	remove nd.

	//set throttle to 0 just in case.
	SET SHIP:CONTROL:PILOTMAINTHROTTLE TO 0.

	return true.
}
