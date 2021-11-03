// --------------------------------------------------------------------------------------------
// Filename: Launch-Orbit.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is to do the Launch until the point of Final Orbit AP
// 02/Nov/2021
// --------------------------------------------------------------------------------------------
parameter FINAL_ORBIT. 			// Sample: 125000 or 150000 or 300000-- Set FINAL_ORBIT to your desired circular orbit
LOG "START: Launch-Orbit.c" to LOG_FILE.

set FINAL_ORBIT2 to FINAL_ORBIT.// For Phase-2 falcon stage-2
set FINAL_ORBIT  to 150000. 	//(FINAL_ORBIT/2) - For Phase-1 falcon stage-1
set phase to 0.
set vel to 0.
set str_vehicle to "".

//DEFINE SN20-Profile TARGET
set LandingTarget to latlng(23.12854, -159.982839).
runpath("boot/common.c").

// LOOP: LAUNCH-Trusting:
if vehicle_sub_type = "SN20-Profile"
	set lat_correction to -6. // -6 = HEADING: 90+6

// --------------------------------------------------------------------------------------------
function main_liftoff
{
	LOG "main_liftoff()" to LOG_FILE.
	if vehicle_company = "SpaceX"
	{
		if (vehicle_type = "Falcon Heavy" or vehicle_type = "StarShip")
			set str_vehicle to vehicle_type.
		else if vehicle_type = "SN16-Profile1"
			set str_vehicle to "StarShip".
		else
			set str_vehicle to "Falcon".
		
		if vehicle_type = "Falcon Heavy" OR vehicle_type = "Crew Dragon 2" OR vehicle_type = "F9v1.2B5"
		{
			// Vehicle Release Auto Sequence:
			PRINT " ".
			PRINT "T-04:15 MVac and M1D fuel bleed has started. ".
			PRINT "T-04:10 ST-1 LOX throttling to close up".
			PRINT "T-03:40 M1D LOX load complete".
			PRINT "T-03:30 P-1&LOX throttling to close up".
			PRINT "T-03:25 P-1&LOX load completed".
			PRINT "T-03:15 ST-2 TVC motions nominal".
			PRINT "T-02:55 POGO bleed verification".
			PRINT "T-02:50 Center-core LOX load complete".
			PRINT "T-02:00 ST-2 LOX load complete".
			PRINT "T-01:50 Falcon decouple is complete".
			PRINT "T-01:40 "+str_vehicle+" is on internal power".
			if vehicle_type = "Falcon Heavy"
				PRINT "T-01:30 Vehicles in self alignment".
			PRINT "T-01:25 "+str_vehicle+" gas load complete".
			PRINT "T-01:15 M1D-Fuel bleed complete".
			if vehicle_type = "Falcon Heavy" 
				PRINT "T-01:15 M1D Engine shells is complete".
			PRINT "T-01:00 (Final FTS): READY FOR LAUNCH".
			PRINT "T-00:55 "+str_vehicle+" (computer) is on start-up".
			PRINT "T-00:45 STAGE-1&2 Pressurization for flight".
			PRINT "T-00:30 (T-30 Seconds)".
			PRINT "T-00:30 [Launch Director]: GO FOR LAUNCH".
			PRINT "T-00:15 "+str_vehicle+" is configured for flight".
			PRINT "T-00:15 (T-15 Seconds)".
			PRINT "T-00:14 Standby for Turnarround Count".
			if vehicle_type = "Falcon Heavy"
				PRINT "T-00:05 Side Boosters".
			PRINT "T-00:03 Ignition".

			PRINT " ".
		}
		
		PRINT "vehicle_sub_type: "+vehicle_sub_type.
		PRINT " ".
		
		if vehicle_type = "Crew Dragon 2"
		{
			set thrust to 1.
			AG3 ON. //Retract Crew Tower
			if (KUniverse:ActiveVessel = SHIP) STAGE.	//Water run...
		}

		if vehicle_type <> "SaturnV" and vehicle_type <> "StarShip" //and vehicle_type <> "Space4"
			RCS ON.

		if vehicle_type = "F1-M1" or 
		   vehicle_sub_type = "Falcon Heavy LEM" or 
		   vehicle_type = "Crew Dragon 2" or
		   vehicle_type = "SaturnV" or
		   vehicle_type = "SN9-Profile1" or 
		   vehicle_type = "SN16-Profile1" or 
		   vehicle_type = "StarShip" or
		   vehicle_type = "Space4"
			SAS OFF.
		else
			SAS ON.
	} else
		set str_vehicle to vehicle_type.
	
	if (status = "PRELAUNCH" or status = "LANDED" or status = "SPLASHED") //KSP have so many bugs...
	{
		
		if vehicle_company = "SpaceX" 
		{
			AG1 ON. //TOGGLE
			if Release_Tower_Clamp
				Print "(Release Tower Clamp)".
		}
		set aim_cowntdown to 5.
		
		if vehicle_type = "StarShip"
		{
			//AG8 ON. //catch arms
			AG7 ON. //crew
			set aim_cowntdown to 10.
		}
		
		FROM {local countdown is aim_cowntdown.} UNTIL countdown = 0 STEP {SET countdown to countdown - 1.} 
		DO { PRINT "..." + countdown. WAIT 1. }

		CLEARSCREEN. PRINT " ".PRINT " ".	
		update_phase_title("[ ] IGNITION...", 0, false).
		if vehicle_type = "SN9-Profile1"
			set thrust to 0.875. //ALL ENGINES: START IGNITION!
		else
			set thrust to 1. 	//ALL ENGINES: START IGNITION!
		
		// --------------------------------------------------------------------------------------------
		if vehicle_type = "Crew Dragon 2" or vehicle_type = "Falcon Heavy"
			WAIT 1.
			
		if (KUniverse:ActiveVessel = SHIP) STAGE.		//TOWER
		if vehicle_company = "SpaceX" and Release_Tower_Clamp
			Print "(Strongback Retracted)".

		if vehicle_type = "Space4"
		{
			AG4 ON. WAIT 1. //Nucler Reactor on
			AG5 ON. WAIT 1. //Vertical Jet Engines
			AG1 ON. WAIT 1. //Jet Engines
			AG2 ON. WAIT 1. //Multi/SPI Jet Engines
			AG8 ON. WAIT 1. //VERTICAL RS-25
		} 
			
		if vehicle_type = "Crew Dragon 2"
			WAIT 1.										//CD2: TOWER+Liftoff
		else {
			WAIT 3.
			if vehicle_type = "SaturnV"
				WAIT 5.									//SaturnV: 3+5=8 seconds total to reach max power
				
			if vehicle_company <> "PMBORG"
				if (KUniverse:ActiveVessel = SHIP) STAGE.	//Liftoff Stage!
		}
	}
}//main_liftoff

// --------------------------------------------------------------------------------------------
function do_stage
{
	LOG "do_stage()" to LOG_FILE.
	if vehicle_type = "Crew Dragon 2" and KUniverse:ActiveVessel <> SHIP
		AG7 ON. //Decouple and Activate Engine
	else
	{
		UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		if vehicle_company <> "PMBORG"
			stage.
		
		if vehicle_type = "Falcon Heavy" 
			AG6 ON. //Toggle: FH Boosters separator
	}
}

// --------------------------------------------------------------------------------------------
function check_if_we_need_new_stage
{
	if throttle > 0 and maxthrust = 0
	{
		do_stage().
		PRINT "(Staging...)" at (0,2).
	}
	
	if alt:radar > 1000 and SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM" and maxthrust < 10000 and stage:number = 10
	{
		do_stage().
		engines_thrustlimit_to (100).	//Main Core back to 100% now
	}
}

// --------------------------------------------------------------------------------------------
function check_fairing_sep 
{
	if (((vehicle_type = "F9v1.2B5") or (vehicle_type = "F1-M1")) and altitude > FAIRSEP and phase = 0) or
	   (vehicle_type = "Falcon Heavy" and altitude > FAIRSEP and phase = 1)	// MECO1 done?
	{
		if (KUniverse:ActiveVessel = SHIP) 
		{
			STAGE.
			wait 1.
			AG3 ON. 		//For some special cases out of focus
			set phase to 2.	//fairing-sep
		}
	}
	if vehicle_sub_type = "Falcon Heavy LEM" and altitude > FAIRSEP and phase < 2
	{
		AG3 ON. //Special: Faring Decouple
		set phase to 2.	//fairing-sep
	}
	if vehicle_type = "SaturnV" and altitude > 100000 and phase < 1
	{
		stage. 		 	//LES: launch escape system
		set phase to 1.	 
	}
}

function GoSN9
{
	//Align performance with real telemetry data:
	if (update_th)
	{
		if alt:radar < 1000
			set thrust to 1.0625*((mass*g)/maxthrust).
		else
			set thrust to 1.039*((mass*g)/maxthrust).
	}
	
	if alt:radar > 3600 and profile_stage = 0
	{
		sn11_test_profile_deactivate_engine1().
		set profile_stage to 0.5.
		PRINT "( Shutdown 1st engine )" at (0,5+index2).
		set t_now to (TIME:SECONDS-TakeOffTime).
		set update_th to false.
	}

	//Simulate the acc delay on Raptor engines:
	if profile_stage = 0.5
		if (TIME:SECONDS-TakeOffTime) > t_now+2
		{
			set update_th to true.
			set profile_stage to 1.
		}

	PRINT "P-ST: "+profile_stage at (30,2).
}


// function Clamp {
	// PARAMETER value.
	// PARAMETER lower.
	// PARAMETER upper.
 
	// return Max(lower, Min(upper, value)).
// }
// function GetLaunchAzimuthInertial {
	// PARAMETER targetInclination.
	// PARAMETER launchLatitude.
 
	// return Arcsin(Clamp(Cos(targetInclination)/Cos(launchLatitude), -1, 1)).
// }
// set targetInclination to mission_target:orbit:inclination.
// function GetLaunchAzimuthRotating {
	// set  launchLatitude to Ship:Latitude.
	// set  targetAltitude to Altitude.
 
	// local azimuth_inertial to GetLaunchAzimuthInertial(targetInclination, launchLatitude).
 
	// local v_equatorial to 2*Constant():Pi*Body:Radius / Body:RotationPeriod.
 
	// local v_orbit to Sqrt(Body:Mu / (Body:Radius + targetAltitude)).
	// local v_xrot to v_orbit * sin(azimuth_inertial) - v_equatorial * cos(launchLatitude).
	// local v_yrot to v_orbit * cos(azimuth_inertial).
 
	// return Arctan(v_xrot/v_yrot).
// }

// declare t0 to time:seconds.	//returns compass direction to launch to a desired inclination:
// declare dH to 0.
// declare oldH to 0.		
// declare function directionFindPD 
// {
  
  // set t to time:seconds.	//iterate time step, only necessary for dH:
  // set dT to t-t0.
  // set t0 to t.
  
  
  // set dH to (ship:heading-oldH)/dT.	//set dH to change in heading:
  // set oldH to ship:heading.
  
  // return 90+.01*(ship:heading)+.01*dH.	//return Kp*(setpoint-P)+Kd*D
// }

declare function directionFindPD
{
	return 104.	//Magic Number to GetLaunchAzimuthRotatingHeading to Mars
}

set new_PITCH to 10. //Space4

function GoSpace4
{
		if vehicle_type = "Space4" and altitude > 200 and Space4 = 0
		{
			RCS OFF.
			GEAR OFF. WAIT 0.1.
			AG8 OFF.  WAIT 0.1. //VERTICAL RS-25
			set Space4 to 1.
			update_phase_title("@ALT:200m, VERT. RS-25: OFF + GEAR OFF", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and altitude > 300 and Space4 = 1
		{
			//AG4 ON. WAIT 1. 	//Nucler Reactor on
			AG5 OFF. WAIT 0.1. 	//Vertical Jet Engines
			//AG1 ON. WAIT 0.1. //Jet Engines
			//AG2 ON. WAIT 1. 	//Multi/SPI Jet Engines
			//AG8 OFF. WAIT 0.1.//VERTICAL RS-25
			set Space4 to 2.
			set new_PITCH to 30.
			update_phase_title("@ALT:300m, PITCH: "+new_PITCH+" / VERT. Jet Engines: OFF", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and altitude > 2000 and Space4 = 2
		{
			set Space4 to 3.
			set new_PITCH to 70.
			update_phase_title("@ALT:002km, PITCH: "+new_PITCH, 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and altitude > 12000 and Space4 = 3
		{
			set Space4 to 4.
			AG7 ON. wait 0.1.
			AG7 OFF. wait 0.1.
			update_phase_title("@ALT:012km, SET ENGINES TO: ROCKET MODE", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and altitude > 24000 and Space4 = 4
		{
			set Space4 to 5.
			AG1 OFF. wait 0.1.
			update_phase_title("@ALT:024km, JET ENGINES: OFF", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and altitude > 52000 and Space4 = 5
		{
			set Space4 to 6.
			AG9 ON. wait 0.1.
			update_phase_title("@ALT:052km, VAC ENGINE: ON", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and apoapsis > 140000 and Space4 = 6
		{
			set Space4 to 7.
			AG2 OFF. wait 0.1.
			set new_PITCH to 15.
			update_phase_title("@AP:140km, ATM ROCKET ENGINES: OFF", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and apoapsis > 250000 and Space4 = 7
		{
			set Space4 to 8.
			set new_PITCH to 2.
			update_phase_title("@AP:250km, PITCH: "+new_PITCH, 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and periapsis > -1000000 and Space4 = 8
		{
			set Space4 to 9.
			set new_PITCH to 0.
			update_phase_title("@PE:-1000km, PITCH: "+new_PITCH, 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and periapsis > 140000 and Space4 = 9
		{
			set Space4 to 10.
			update_phase_title("@PE:140km, ORBIT: 140km", 0, true, 6, 0).
		}
		if vehicle_type = "Space4" and apoapsis > 1000000 and Space4 = 10
		{
			set Space4 to 11.
			AG9 OFF. wait 0.1.
			LOCK STEERING TO PROGRADE. 	//UNLOCK STEERING.
			set profile_stage to 1.
			update_phase_title("@AP:1000km, ORBIT: ", 0, true, 6, 0).
		}
		
		LOCK STEERING TO HEADING(directionFindPD(), new_PITCH).
}

// Vehicle Release Auto Sequence:
// --------------------------------------------------------------------------------------------
LOG "START: Vehicle Release Auto Sequence" to LOG_FILE.
set thrust to 0.
lock throttle to thrust.

if vehicle_type = "Space4" {
	LOCK STEERING TO HEADING(90, new_PITCH).
	set target to mission_target.
}
else
	if vehicle_type <> "SN16-Profile1"
		LOCK STEERING TO up + R(0,0,180). //UP
	
set initialmass to mass.
set Cd to .20075*.008.
set D to (mass*g)/Qmax.
set deltaReduction to 0.

PRINT "Takeoff MASS: "+ROUND (SHIP:MASS)+" t".
PRINT "Final Orbit: "+ ROUND(FINAL_ORBIT2/1000)+" km".
PRINT "Qmax: "+ ROUND(Qmax).
PRINT "[USER-CONFIG.KS]-STAGE1_LAND_ON: "+STAGE1_LAND_ON.

set TakeOffTime to TIME:SECONDS. //(define: Secure for reboots)
set last_value1 to 0.
set Aceleration_value1 to 0.

// Initial needed values, later:
set p0 to 1.223125.
set e to constant:e.
set q to 0.
set mphase to 0.
set Space4 to 0.

if alt:radar < 200
{
	//WAIT for the GO!
	// --------------------------------------------------------------------------------------------
	main_liftoff().
	
	//LIFTOFF...
	// --------------------------------------------------------------------------------------------	
	set value1 to 0.
	set last_value1 to 0.
	set TakeOffTime to TIME:SECONDS.

	if vehicle_type <> "SN9-Profile1"
		set thrust to 1.

	CLEARSCREEN.
	update_phase_title("[1] LIFTOFF...",0, false).
	
	DELETEPATH("FLIGHT_LOG.txt").
	LOG   "TIME,   VELO,   R:ALT,  Acel,   Q" to FLIGHT_LOG_FILE.

	PRINT "Q-Max" 				at (0,2).
	PRINT "Dynamic Pressure" 	at (0,3).
	PRINT "q/Qmax" 				at (0,4).
	if vehicle_type = "Space4"
		PRINT "Space4 Phase:" 				at (0,5).
	set index2 to 6.

	set profile_stage to 0.

	// LOOP: UNTIL HALF Qmax
	// --------------------------------------------------------------------------------------------
	set update_th to true.
	until q > Qmax*.50 or profile_stage >= 1	// END CYCLE AT: qmax 50%
	{ 
		set H to altitude/(-5000).
		set p to p0*(e^H).
		set q to .5*p*(verticalspeed^2). // pd = 1/2 ρ v^2
		
		PRINT ROUND(Qmax) at (22,2).
		PRINT ROUND(q) at (22,3).
		set q_qmax to q/Qmax*100.
		PRINT ROUND(q_qmax,2)+ " %       " at (22,4).
		if vehicle_company = "SpaceX" and not splash_landing
			PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,6).
		
		if vehicle_type = "Space4"
		{
			PRINT Space4 				at (22,5).
			GoSpace4().
		}
		else
		{
			if alt:radar > 130 and alt:radar < 1000
			{
				if vehicle_type <> "SN9-Profile1"
					PRINT "( Tower is cleared )" at (0,5+index2).
				if vehicle_type = "Crew Dragon 2" //OR vehicle_type = "Space4"
					SAS ON.
			}
			else
				if vehicle_type <> "SN9-Profile1"
					PRINT "                    " at (0,5+index2).
					
			//SN9-Profile1 only:
			if alt:radar > 100 and vehicle_type = "SN9-Profile1"
				GoSN9().
		}
	
		set vsurf to velocity:surface.
		set Vsx to vsurf:x.
		set Vsy to vsurf:y.
		set Vsz to vsurf:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).	
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel).
	}.

	// Throttle is reduced to maintain a constant terminal velocity.
	// --------------------------------------------------------------------------------------------
	update_phase_title("[2] LAUNCH-Trusting",1, false).
	PRINT "Throttle" at (0,1+index2).
	PRINT "Maxthrust: " at (0,2+index2).
	PRINT "delta: " at (0,3+index2).
	
	set x to 0.
	set phase to 0.
	
	//SETUP Steering:
	steering_falcon(90).
	LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).
	
	// --------------------------------------------------------------------------------------------
	until altitude > 30000 or profile_stage >= 3
	{
		if machVal > 0.8 and mphase = 0
		{
			update_phase_title("[3] Transonic Flight", 0, false).
			set mphase to 1.
		}
		if machVal > 1.2 and mphase = 1
		{
			update_phase_title("("+str_vehicle+" is SuperSonic)",1, false).
			set mphase to 2.
		}
		
		// set_max_delta_curve:
		set I to altitude/(15000).
		if vehicle_type = "Falcon Heavy"
			set delta to (1*(e^I)*(-1))*1.2.
		else 
			set delta to (1*(e^I)*(-1)).
  
		set H to altitude/(-5000).
		set p to p0*(e^H).
		set vsurf to velocity:surface.
		set Vsx to vsurf:x.
		set Vsy to vsurf:y.
		set Vsz to vsurf:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).	
		set q0 to q.			//last value
		set q to .5*p*(vs2).	//new value
		set err to .002.
		set error to 1-err*(q-Qmax).
		
		if q < q0 //and phase >= 1 
		{
			PRINT "( Passed MAXQ! )              " at (0,6+index2).
			set phase to 2. //Passed MAXQ
		}
		
		if 	maxthrust > 0 {
			set Drag to ((Qmax*Cd*mass)/maxthrust).
			set Weight to (mass*g)/maxthrust.	
		} else {
			set Drag to ((Qmax*Cd*mass)/0.01). 
			set Weight to (mass*g)/0.01.		
		}

		//SN9-Profile1 only:
		if vehicle_type = "SN9-Profile1"
			set delta to -delta.
			
		if vehicle_type = "StarShip" or vehicle_type = "SN16-Profile1"
			steering_falcon(90-delta, lat_correction, -180).
		else
			steering_falcon(90-delta, lat_correction).
		
		set tThrust to (Drag+ Weight)*error.
		if  tThrust < 0.85 
		{
			set tThrust to 0.85.
			if phase = 0 {
				set phase to 1.
				PRINT "( Throttle down to reduce drag losses )" at (0,4+index2).
			}
		}
		if  tThrust >= 1
		{
			set tThrust to 1.
			
			if phase = 1
				set phase to 2.
			
			if phase = 2
			{
				PRINT "                                       " at (0,4+index2).
				PRINT "( Continuing downrange acc )" at (0,5+index2).
				PRINT "( Passed MAXQ! )          " at (0,6+index2).
			}
		}

		//SN9-Profile1 only:
		if vehicle_type = "SN9-Profile1"
		{
			if alt:radar > 8900 and profile_stage = 1
			{
				sn11_test_profile_deactivate_engine2().
				set profile_stage to 2.
				PRINT "( Shutdown 2nd engine )" at (0,5+index2).
			}
			if altitude >= 10600 and profile_stage = 2
			{
				set profile_stage to 3.
				PRINT "( Start Flip to Horizontal )" at (0,5+index2).
			}
			PRINT "P-ST: "+profile_stage at (30,2).
			set tThrust to 1.036*((mass*g)/maxthrust).
		}
		
		set thrust to (tThrust).
			
		if delta <= -1 and x < 1 
		{
			PRINT "( Begin downrange acc )" at (0,5+index2).
			set x to x+1.
		}.
		
		if vehicle_company = "SpaceX" and not splash_landing
			PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,index2).
			
		PRINT ROUND(tThrust*100,1)+"%                " 	at (22,1+index2).	
		PRINT ROUND(Qmax)+"  " 							at (22,2).
		PRINT ROUND(q)+"  " 							at (22,3).
		PRINT ROUND(q/Qmax*100,2)+ " %       " 			at (22,4).
		PRINT ROUND(maxthrust)+ " kN    " 				at (22,2+index2).
		PRINT ROUND(delta)+ "    " 						at (22,3+index2).
	
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel).
	}.

	if vehicle_type <> "SN9-Profile1" 
	{
		// --------------------------------------------------------------------------------------------
		SAS OFF.
		CLEARSCREEN.
		PRINT " ".PRINT " ".
		update_phase_title("[5] ASCENT",1,false).
		set thrust to 1.
		set x to 0.
		RCS OFF.
		set Vs2 to 0.
		set phase to 0.
		SET NAVMODE TO "ORBIT".

		// After 30km, the effects of drag are minimal so thrust to 100% and Burn until MECO1:
		PRINT "mass: " 				at (0,4).
		PRINT "thrust: " 			at (0,5).
		PRINT "apoapsis: " 			at (0,6).
		PRINT "delta: " 			at (0,7).
		PRINT "VERTICALSPEED: " 	at (0,8).
		PRINT "mphase: " 			at (0,9).
		PRINT "phase: " 			at (0,10).
		//PRINT "deltaReduction: " 	at (0,11).
		
		LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).
		// LOOP: ASCENT:
		// --------------------------------------------------------------------------------------------
		until (Vs2 >= MECO1) or (apoapsis >= FINAL_ORBIT2) or phase = 3 //(Reusable) or (Non Reusable Mission) or (on stage-2 burn)
		{
			set delta to set_max_delta_curve().
			if vehicle_type = "StarShip" or vehicle_type = "SN16-Profile1"
				steering_falcon(90-delta, lat_correction, -180).
			else
				steering_falcon(90-delta, lat_correction).

			set vorb to velocity:orbit.
			set Vsx to vorb:x.
			set Vsy to vorb:y.
			set Vsz to vorb:z.
			set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).
			
			if vehicle_company = "SpaceX" and not splash_landing
				PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,3).
				
			PRINT ROUND(mass)+" t   " 			 at (22,4).
			PRINT ROUND(thrust*100,1)+" %   " 			 at (22,5).
			PRINT ROUND(apoapsis/1000)+" km    " at (22,6).
			PRINT ROUND(delta)+"    " 			 at (22,7).
			PRINT ROUND(VERTICALSPEED)+" m/s   " at (22,8).
			PRINT mphase 						 at (22,9).
			PRINT phase 						 at (22,10).
			//PRINT deltaReduction 				 at (22,11).
			
			if (Aceleration_value1 > 30)
			{
				if (mphase = 2) 
				{
					//PRINT "( Throttle down to avoid stress on vehicle )" at (0,11).
					set mphase to 3. // Acceleration Relation... to avoid stress on vehicle.
				}
				
				if vehicle_type = "StarShip"
					set thrust to 4*0.81*((mass*g)/maxthrust). //Max acc: 3g
				else
					set thrust to (thrust).
			}
			
			if vehicle_type = "F1-M1" or 
			   vehicle_sub_type = "Falcon Heavy LEM" or
			   vehicle_type = "SaturnV"
				check_fairing_sep().

			set vel to SQRT(Vs2).
			update_atmosphere (altitude, vel).
			log_data (vel).
			check_if_we_need_new_stage().
		}.		

		if vehicle_type <> "StarShip"
			RCS ON.
			
		SAS OFF.
		if SHIP_NAME <> "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM" and 
		  SHIP_NAME <> "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM2" and
		  vehicle_type <> "SaturnV"
		{ 
			set thrust to 0.01.
			WAIT 1.
		}
	}
}

set warp to 0.

set vorb to velocity:orbit.
set Vsx to vorb:x.
set Vsy to vorb:y.
set Vsz to vorb:z.
set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).
set vel to SQRT(Vs2).

//F9/FH: STAGE-1/BOOSTER SEP
// --------------------------------------------------------------------------------------------
if altitude*1.1 < FINAL_ORBIT2 and vehicle_type <> "SN9-Profile1" and vehicle_type <> "SN16-Profile1" and vel < 3000
{
	LOG "START: STAGE-1/BOOSTER SEP" to LOG_FILE.
	CLEARSCREEN.
	PRINT " ".PRINT " ".
	
	// OK: >=3 for F9 - CHECK: FH...
	if ( (STAGE:NUMBER >= 3) or (vehicle_type = "StarShip") ) and (MECO1 < 9999^2)  //A reusable booster Mission ?
	{
		update_phase_title("[6] STAGING...",1,false).
		PRINT "1st ORBIT: "+FINAL_ORBIT+"km, Done!".

		//BOOSTER WAKE MSG UP! --------------------------------------------------------
		if vehicle_type = "Falcon Heavy" 
		{
			//[FH]:SEND PROCESSOR ID TO BOOSTER
			IF PROCESSOR_STAGE1L:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
			{
			  PRINT "PROCESSOR_STAGE1: Message sent!".
			  WAIT 1.
			}
			//[FH]:SEND PROCESSOR ID TO BOOSTER
			IF PROCESSOR_STAGE1R:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
			{
			  PRINT "PROCESSOR_STAGE1: Message sent!".
			  WAIT 1.
			}
		} else {
			//[F9]:SEND PROCESSOR ID TO BOOSTER
			IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
			{
			  PRINT "PROCESSOR_STAGE1: Message sent!".
			  WAIT 1.
			}	
		}

		//STAGE BOOSTERS --------------------------------------------------------------
		//DEBUG:
		//KUniverse:PAUSE().
		
		RCS ON.
		if vehicle_type = "StarShip"
		{
			LOG  "STAGE SS" to LOG_FILE.
			WAIT 0.1.
			if (KUniverse:ActiveVessel = SHIP) STAGE.	//[SS]
			WAIT 0.1.
			AG2 ON.
			WAIT 0.1.
			activateVesselProbe(0, "Ship").
		} 
		else 
		{
			if vehicle_type = "Falcon Heavy"
			{
				LOG  "STAGE FH" to LOG_FILE.
				AG6 ON. //(Toggle: FH Boosters separator)
				WAIT 0.5.
				if (KUniverse:ActiveVessel = SHIP) STAGE.	//[FH]
				WAIT 5.
			} else {
				LOG  "STAGE ST1" to LOG_FILE.
				st1_stage().								//[F9]
			}
			activateVesselProbe().
		}
	}
	
	// ORBIT SETUP 
	// --------------------------------------------------------------------------------------------
	CLEARSCREEN.
	update_phase_title("[7] ORBIT PHASE I",1,false).
	set thrust to 0.25.	// Stage-2 Initial Slow Burn:
	if vehicle_type = "Crew Dragon 2" or vehicle_type = "Falcon Heavy"
		WAIT 1.
	else
		WAIT 3.

	SAS OFF.

	function print_orbit_status 
	{
		set y to 3.
		PRINT "Periapsis: " at (0,y+0). 	 
		PRINT "Apoapsis: " at (0,y+1). 		 
		PRINT "Eta:apoapsis: " at (0,y+2). 	 
		PRINT "Eccentricity: " at (0,y+3). 	 
		PRINT "Altitude: " at (0,y+4). 		 
		PRINT "Phase: " at (0,y+5). 		 
		PRINT "mphase: " at (0,y+6).		 
		PRINT "deltaReduction: " at (0,y+7). 
		PRINT "throttle(maxthrust): " at (0,y+8). 
		PRINT "HEADING: " at (0,y+9). 	 	
		PRINT "PITCH: " at (0,y+10). 	 	
	}
	
	print_orbit_status ().
	
	function update_orbit_status 
	{
		set y to 3.

		PRINT ROUND(periapsis/1000,1)+" km    " 	at (22,y+0).
		PRINT ROUND(apoapsis/1000,1)+" km    " 	at (22,y+1).
		PRINT ROUND(eta:apoapsis,1)+" s    " 		at (22,y+2).
		PRINT ROUND(SHIP:ORBIT:ECCENTRICITY,3)+"" 	at (22,y+3).
		PRINT ROUND(Altitude/1000,1)+" km    "  	at (22,y+4).
		PRINT phase 								at (22,y+5).
		PRINT mphase 								at (22,y+6).
		PRINT deltaReduction 						at (22,y+7).
		PRINT ROUND(throttle*100) + "% ("+ROUND(maxthrust)+")     " at (22,y+8).
		PRINT ROUND (vessel_compass(),1)+" deg.   " 	at (22,y+9).
		PRINT ROUND (vessel_pitch(),1)+" deg.   " at (22,y+10).
	}

	if vehicle_type = "Falcon Heavy"
		engines_thrustlimit_to (100).	//Main Core back to 100% now

	// --------------------------------------------------------------------------------------------
	set thrust to 1.
	RCS OFF.
	set phase to 0.
	set deltaReduction to 0.
	// UNLOCK STEERING. wait 0.1.
	// LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll). wait 0.1.
	
	function do_orbit 
	{
		set do_break to false.

		set eta_apoapsis to eta:apoapsis.
		set vorb to velocity:orbit.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).
		
		if vehicle_type = "Crew Dragon 2" and verticalspeed < 0
			activateVesselProbe().	//Time to Switch to ST-1
			
		if vehicle_sub_type = "Falcon Heavy" and phase=0 and (Vs2 > MECO2 or altitude > FAIRSEP)
		{	// FH:CORE Booster SEP:
			RCS ON.
			set thrust to 0.
			WAIT 1.
			if (KUniverse:ActiveVessel = SHIP) 
			{
				STAGE.
				WAIT 3.
				if (KUniverse:ActiveVessel = SHIP) 
					STAGE. 
				set phase to 1. // Booster SEP}
				set thrust to 1.
				RCS OFF.
				if vehicle_type = "Falcon Heavy"
					SET Vdeg to 90-75.
				else
					SET Vdeg to 90-85.
				WAIT 1.
			}
		}

		if vehicle_sub_type = "SN20-Profile"  
		{
			if (ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT) //KUniverse:ActiveVessel = SHIP and 
			{
				//set LandingTarget to latlng(23.12854, -159.982839).
				set ADDONS_TR_IMPACTPOS to ADDONS:TR:IMPACTPOS.
				
				SET landingDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
				PRINT "landingDist: "+ROUND(landingDist/1000,1) + "  km    " at (0,16).
				PRINT "[IMPACTPOS]: "+ROUND (ADDONS_TR_IMPACTPOS:LAT,3)+" " +ROUND (ADDONS_TR_IMPACTPOS:LNG,3) at (0,17).
			}
			if periapsis >= 140000
				set do_break to true.
		}

		if vehicle_type <> "Crew Dragon 2" and vehicle_company <> "PMBORG"
			check_fairing_sep().
			
		return do_break.
	}
	
	// LOOP: ORBIT PHASE I (Maximizing: Horizontal Aceleration)
	// --------------------------------------------------------------------------------------------
	UNTIL periapsis > 0 and apoapsis < FINAL_ORBIT2
	{
		//UPDATE FROM: steering_falcon(90-delta, lat_correction).
		//TO:
		SET steeringVdeg to set_Vdeg().

		if vehicle_type = "StarShip" or vehicle_type = "SN16-Profile1"
		{
			set steeringVroll to -180.					// 0 = Zero Rotation
			SET steeringDir TO 90-lat_correction.	// W/E
		} else {
			set steeringVroll to -270.				// -270 = Zero Rotation
			SET steeringDir TO 90+lat_correction.	// W/E
		}
		LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).
		
		if do_orbit()
			break.
		
		if (Aceleration_value1 > 30.75)
		{
			//set deltaReduction to (deltaReduction+0.001).
			if (mphase = 2) 
			{
				PRINT "( Throttle down to reduce acceleration, to avoid stress on vehicle )" at (0,5).
				set mphase to 3. // Acceleration Relation... to avoid stress on vehicle.
			}
			set thrust to (thrust-deltaReduction).
		}
		
		update_orbit_status().
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel).
		check_if_we_need_new_stage().
		
		set dx to eta:apoapsis.
		if dx > eta_apoapsis and stage:number < 7
			break.
	}

	if vehicle_type = "F9v1.2B5"
		activateVesselProbe().			//Time to Switch to ST-1

	if vehicle_type = "StarShip"
		activateVesselProbe(0, "Ship").
			
	update_phase_title("[7] ORBIT PHASE II",1,false). 
	// LOOP: ORBIT PHASE II (Maximizing Apoapsis Increase)
	// --------------------------------------------------------------------------------------------
	UNTIL (apoapsis >= FINAL_ORBIT2) 
	{
		if verticalspeed < 0 
			SET steeringVdeg to (-verticalspeed/8).	// UP/DOWN: Vertical = 90 (magical number: 8 tune precision)
		else
			SET steeringVdeg to set_Vdeg().
		
		if vehicle_type = "StarShip" or vehicle_type = "SN16-Profile1"
		{
			set steeringVroll to -180.				// 0 = Zero Rotation
			SET steeringDir TO 90-lat_correction.	// W/E
		} else {
			set steeringVroll to -270.				// -270 = Zero Rotation
			SET steeringDir TO 90+lat_correction.	// W/E
		}
		//LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).//steering_falcon(Vdeg).
		
		if do_orbit()
			break.
		
		update_orbit_status().
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel).
		check_if_we_need_new_stage().
	}

	set thrust to 0.
	LOCK STEERING TO PROGRADE. 	//UNLOCK STEERING.
	PRINT "Orbit insertion completed".
	WAIT 5.

	if orbit_type = "GSO"
	{
		//25% now on.
		LIST ENGINES IN myVariable.
		FOR eng IN myVariable {
			set eng:THRUSTLIMIT to 25.
			if eng:HASGIMBAL
				set eng:Gimbal:LIMIT to 25.
		}.
	}
}


set warp to 0.
wait 2.
LOG   "END: Launch-Orbit.c" to LOG_FILE.
LOG   "-------------------" to LOG_FILE.