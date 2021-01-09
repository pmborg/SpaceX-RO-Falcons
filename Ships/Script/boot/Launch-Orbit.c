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
// 09/Jan/2021
// --------------------------------------------------------------------------------------------
parameter FINAL_ORBIT. 			// Sample: 125000 or 150000 or 300000-- Set FINAL_ORBIT to your desired circular orbit
set FINAL_ORBIT2 to FINAL_ORBIT.// For Phase-2 falcon stage-2
set FINAL_ORBIT  to 150000. 	//(FINAL_ORBIT/2) - For Phase-1 falcon stage-1
set phase to 0.
set str_vehicle to "".
// --------------------------------------------------------------------------------------------
function main_liftoff
{
	if vehicle_company = "SpaceX"
	{
		if vehicle_type = "Falcon Heavy" 
			set str_vehicle to vehicle_type.
		else
			set str_vehicle to "Falcon".
		
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
		PRINT "T-00:05 Side Boosters Ignition".

		PRINT " ".
		
		if vehicle_type = "Crew Dragon 2"
		{
			set thrust to 1.
			AG3 ON. //Retract Crew Tower
			if (KUniverse:ActiveVessel = SHIP) STAGE.	//Water run...
		}

		if vehicle_type <> "SaturnV"
			RCS ON.

		if vehicle_type = "F1-M1" or 
		   vehicle_sub_type = "Falcon Heavy LEM" or 
		   vehicle_type = "Crew Dragon 2" or
		   vehicle_type = "SaturnV"
			SAS OFF.
		else
			SAS ON.
	} else
		set str_vehicle to vehicle_type.
	
	if (status = "PRELAUNCH" or status = "LANDED" or status = "SPLASHED") //KSP have so many bugs...
	{
		AG1 ON. //TOGGLE
		if vehicle_company = "SpaceX" Print "(Release Tower Clamp)".
		
		FROM {local countdown is 5.} UNTIL countdown = 0 STEP {SET countdown to countdown - 1.} 
		DO { PRINT "..." + countdown. WAIT 1. }

		CLEARSCREEN. PRINT " ".PRINT " ".	
		update_phase_title("[ ] IGNITION...", 0, false).
		set thrust to 1. //ALL ENGINES: START IGNITION!
		
		// --------------------------------------------------------------------------------------------
		if vehicle_type = "Crew Dragon 2" or vehicle_type = "Falcon Heavy"
			WAIT 1.
			
		if (KUniverse:ActiveVessel = SHIP) STAGE.		//TOWER
		if vehicle_company = "SpaceX" 
			Print "(Strongback Retracted)".

		if vehicle_type = "Crew Dragon 2"
			WAIT 1.										//CD2: TOWER+Liftoff
		else {
			WAIT 3.
			if vehicle_type = "SaturnV"
				WAIT 5.									//SaturnV: 8 seconds total to reach max power
			if (KUniverse:ActiveVessel = SHIP) STAGE.	//Liftoff
		}
	}
}

// --------------------------------------------------------------------------------------------
function do_stage
{
	if vehicle_type = "Crew Dragon 2" and KUniverse:ActiveVessel <> SHIP
		AG7 ON. //Decouple and Activate Engine
	else
	{
		UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
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
	// if alt:radar > 1000 and SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM2" and maxthrust < 10000 and stage:number = 6
	// {
		// do_stage().
		// set phase to 3.
	// }
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


// Vehicle Release Auto Sequence:
// --------------------------------------------------------------------------------------------
set thrust to 0.
lock throttle to thrust.
LOCK STEERING TO up + R(0,0,180). //UP

set initialmass to mass.
set Cd to .20075*.008.
set D to (mass*g)/Qmax.
set deltaReduction to 0.

PRINT "Takeoff MASS: "+ROUND (SHIP:MASS)+" t".
PRINT "Final Orbit: "+ ROUND(FINAL_ORBIT2/1000)+" km".
PRINT "Qmax: "+ ROUND(Qmax).
PRINT "[USER-CONFIG.KS]-STAGE1_LAND_ON: "+STAGE1_LAND_ON.


set last_value1 to 0.
set Aceleration_value1 to 0.

// Initial needed values, later:
set p0 to 1.223125.
set e to constant:e.
set q to 0.
set mphase to 0.
set TakeOffTime to TIME:SECONDS. //(define: Secure for reboots)

if alt:radar < 200
{
	//WAIT for the GO!
	// --------------------------------------------------------------------------------------------
	main_liftoff().
	
	//LIFTOFF...
	// --------------------------------------------------------------------------------------------	
	CLEARSCREEN.
	update_phase_title("[1] LIFTOFF...",0, false).
	
	DELETEPATH("FLIGHT_LOG.txt").
	LOG   "TIME,   VELO,   R:ALT,  Acel,   Q" to FLIGHT_LOG.txt.

	set TakeOffTime to TIME:SECONDS.
	set value1 to 0.
	set last_value1 to 0.

	PRINT "Q-Max" 				at (0,2).
	PRINT "Dynamic Pressure" 	at (0,3).
	PRINT "q/Qmax" 				at (0,4).
	set index2 to 6.

	set thrust to 1.	

	// LOOP: UNTIL HALF Qmax
	// --------------------------------------------------------------------------------------------
	until q > Qmax*.50
	{ 
		set H to altitude/(-5000).
		set p to p0*(e^H).
		set q to .5*p*(verticalspeed^2). // pd = 1/2 ρ v^2
		
		PRINT ROUND(Qmax) at (22,2).
		PRINT ROUND(q) at (22,3).
		PRINT ROUND(q/Qmax*100,2)+ " %       " at (22,4).
		if vehicle_company = "SpaceX"
			PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,6).
		
		if alt:radar > 130 and alt:radar < 1000
		{
			PRINT "( Tower is cleared )" at (0,5+index2).
			if vehicle_type = "Crew Dragon 2"
				SAS ON.
		}
		else
			PRINT "                    " at (0,5+index2).
		
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
	
	// LOOP: LAUNCH-Trusting:
	// --------------------------------------------------------------------------------------------
	until altitude > 30000 
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
		set q to .5*p*(vs2).
		set err to .002.
		set error to 1-err*(q-Qmax).

		set q0 to q.
		if q < q0 and phase = 1 
		{
			PRINT "( Passed MAXQ! )              " at (0,10+index2).
			set phase to 1. //Passed MAXQ
		}
		
		if 	maxthrust > 0 {
			set Drag to ((Qmax*Cd*mass)/maxthrust).
			set Weight to (mass*g)/maxthrust.	
		} else {
			set Drag to ((Qmax*Cd*mass)/0.01). 
			set Weight to (mass*g)/0.01.		
		}

		steering_falcon(90-delta).
		
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
			// if vehicle_type = "Falcon Heavy" and mphase = 2 and machVal < 1.8
				// set tThrust to 0.85.
			// else
				set tThrust to 1.
			
			if phase = 1
			{
				PRINT "                                       " at (0,4+index2).
				PRINT "( Continuing gravity turn )" at (0,5+index2).
				PRINT "( Passed MAXQ! )          " at (0,6+index2).
				set phase to 2.
			}
		}

		set thrust to (tThrust).
			
		if delta <= -1 and x < 1 
		{
			PRINT "( Begin gravity turn )" at (0,5+index2).
			set x to x+1.
		}.
		
		if vehicle_company = "SpaceX"
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
	PRINT "deltaReduction: " 	at (0,11).
	
	LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).
	// LOOP: ASCENT:
	// --------------------------------------------------------------------------------------------
	until (Vs2 >= MECO1) or (apoapsis >= FINAL_ORBIT2) or phase = 3 //(Reusable) or (Non Reusable Mission) or (on stage-2 burn)
	{
		set delta to set_max_delta_curve().
		steering_falcon(90-delta).

		set vorb to velocity:orbit.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).
		
		if vehicle_company = "SpaceX"
			PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,3).
			
		PRINT ROUND(mass)+" t   " 			 at (22,4).
		PRINT thrust*100+" %   " 			 at (22,5).
		PRINT ROUND(apoapsis/1000)+" km    " at (22,6).
		PRINT ROUND(delta)+"    " 			 at (22,7).
		PRINT ROUND(VERTICALSPEED)+" m/s   " at (22,8).
		PRINT mphase 						 at (22,9).
		PRINT phase 						 at (22,10).
		PRINT deltaReduction 				 at (22,11).
		
		if (Aceleration_value1 > 30)
		{
			if (mphase = 2) 
			{
				//PRINT "( Throttle down to avoid stress on vehicle )" at (0,11).
				set mphase to 3. // Acceleration Relation... to avoid stress on vehicle.
			}
			set thrust to (thrust-deltaReduction).
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

//F9/FH: STAGE-1/BOOSTER SEP
// --------------------------------------------------------------------------------------------
if altitude*1.1 < FINAL_ORBIT2 
{
	CLEARSCREEN.
	PRINT " ".PRINT " ".
	
	// OK: >=3 for F9 - CHECK: FH...
	if (STAGE:NUMBER >= 3) and (MECO1 < 9999^2)  //A Recovery Mission ?
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
		if vehicle_type = "Falcon Heavy" 
		{
			AG6 ON. //(Toggle: FH Boosters separator)
			WAIT 0.5.
			if (KUniverse:ActiveVessel = SHIP) STAGE.	//[FH]
			WAIT 5.
		} else {
			st1_stage().								//[F9]
		}
		
		activateVesselProbe().
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

	set thrust to 1.
	RCS OFF.
	set phase to 0.
	set deltaReduction to 0.
	// UNLOCK STEERING. wait 0.1.
	// LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll). wait 0.1.
	
	// LOOP: ORBIT PHASE I (Maximizing: Horizontal Aceleration)
	// --------------------------------------------------------------------------------------------
	UNTIL periapsis > 0 and apoapsis < FINAL_ORBIT2
	{
		SET steeringDir TO 90.			// W/E
		SET steeringVdeg to set_Vdeg().
		set steeringVroll to -270.		// -270 = Zero Rotation
		LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).
	
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

		if vehicle_type <> "Crew Dragon 2"
			check_fairing_sep().
		
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
		activateVesselProbe().	//Time to Switch to ST-1
			
	update_phase_title("[7] ORBIT PHASE II",1,false). 
	// LOOP: ORBIT PHASE II (Maximizing Apoapsis Increase)
	// --------------------------------------------------------------------------------------------
	UNTIL (apoapsis >= FINAL_ORBIT2) 
	{
		if verticalspeed < 0 
			SET steeringVdeg to (-verticalspeed/8).	// UP/DOWN: Vertical = 90 (magical number: 8 tune precision)
		else
			SET steeringVdeg to set_Vdeg().
		set steeringVroll to -270.			// Zero Rotation
		SET steeringDir TO 90.	// W/E
		//LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).//steering_falcon(Vdeg).
		
		set vorb to velocity:orbit.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set velocity_orbit to (Vsx^2)+(Vsy^2)+(Vsz^2).
		set Vs2 to (velocity_orbit). //km/h
		
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
		if vehicle_type <> "Crew Dragon 2"
			check_fairing_sep().
		
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