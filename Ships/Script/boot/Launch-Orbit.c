// --------------------------------------------------------------------------------------------
// Filename: Launch-Orbit.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is to do the Launch until the point of Final Orbit AP
// 17/Nov/2020
// --------------------------------------------------------------------------------------------

function main_lifoff
{
	if vehicle_type = "Falcon Heavy" 
	{
		PRINT "Press [ENTER] to START". set ch to terminal:input:getchar().
		CLEARSCREEN.
	
		PRINT "Vehicle Release Auto Sequence".
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
		PRINT "T-01:40 FalconHeavy is on internal power".
		PRINT "T-01:30 Vehicles in self alignment".
		PRINT "T-01:25 FalconHeavy gas load complete".
		PRINT "T-01:15 M1D-Fuel bleed complete".
		PRINT "T-01:15 M1D Engine shells is complete".
		PRINT "T-01:00 (Final FTS): READY FOR LAUNCH".
		PRINT "T-00:55 FalconHeavy (computer) is on start-up".
		PRINT "T-00:45 STAGE-1&2 Pressurization for flight".
		PRINT "T-00:30 (T-30 Seconds)".
		PRINT "T-00:30 [Launch Director]: GO FOR LAUNCH".
		PRINT "T-00:15 FalconHeavy is configured for flight".
		PRINT "T-00:15 (T-15 Seconds)".
		PRINT "T-00:14 Standby for Turnarround Count".
		PRINT "T-00:05 Side Boosters Ignition".
	}

	// if vehicle_type = "F9v1.2B5"
	// {
		// PRINT "Press [ENTER] to START". set ch to terminal:input:getchar().
		// CLEARSCREEN.
	// }

	PRINT " ".
	PRINT "Press [ENTER] to START". set ch to terminal:input:getchar().
	CLEARSCREEN. PRINT " ".PRINT " ".	
	
	if vehicle_type = "Crew Dragon 2"
	{
		set thrust to 1.
		AG3 ON. //Retract Crew Tower
		if (KUniverse:ActiveVessel = SHIP) STAGE.	//Water run...
	}

	RCS ON.
	SAS ON.
	if status = "PRELAUNCH" 
	{
		update_phase_title("[ ] COUNTING DOWN...", 0, false).
		AG1 ON. //TOWER...!
		Print "(Release Tower Clamp)".
		
		FROM {local countdown is 5.} UNTIL countdown = 0 STEP {SET countdown to countdown - 1.} 
		DO { PRINT "..." + countdown. WAIT 1. }

		update_phase_title("[ ] IGNITION...", 0, false).
		set thrust to 1. //All ENGINES...
		
		if vehicle_type = "Crew Dragon 2" or vehicle_type = "Falcon Heavy"
			WAIT 1.
			
		if (KUniverse:ActiveVessel = SHIP) STAGE.	//Liftoff
		Print "(Strongback Retracted)".

		if vehicle_type = "Crew Dragon 2"
			WAIT 1.
		else {
			WAIT 3.
			if (KUniverse:ActiveVessel = SHIP) STAGE.
		}
	}
}

runpath("boot/atm.c").

parameter FINAL_ORBIT. 			// Sample: 125000 or 150000 or 300000-- Set FINAL_ORBIT to your desired circular orbit
set FINAL_ORBIT2 to FINAL_ORBIT.// For Phase-2 falcon stage-2
set FINAL_ORBIT  to 150000. 	//(FINAL_ORBIT/2) - For Phase-1 falcon stage-1

set thrust to 0.
lock throttle to thrust.
LOCK STEERING TO up + R(0,0,180).

set initialmass to mass.
// set r to altitude+BODY(DEFAULT_KSC):radius. //600000 (KERBIN)
// set GM to BODY(DEFAULT_KSC):mu. 			//GM = 3.5316*(10^12). (KERBIN)
// set g to GM/(r^2).							//Calculate g = 9.81
//set Qmax to .5*1.2*(100.9^2).
set Cd to .20075*.008.
set D to (mass*g)/Qmax.
set deltaReduction to 0.

PRINT "Takeoff MASS: "+ROUND (SHIP:MASS)+" t".
PRINT "Final Orbit: "+ ROUND(FINAL_ORBIT2/1000)+" km".
PRINT "Qmax: "+ ROUND(Qmax).

if vehicle_type = "Falcon Heavy"
{
	//Factory set 60 -> 70%
	LIST ENGINES IN myVariable.
	FOR eng IN myVariable {
		if eng:THRUSTLIMIT = 60
			set eng:THRUSTLIMIT to 70. // Set CORE to 70% Thrust
	}.
}
set last_value1 to 0.
set Aceleration_value1 to 0.

// Initial needed values, later:
set p0 to 1.223125.
set e to constant:e.
set q to 0.
set mphase to 0.
set TakeOffTime to TIME:SECONDS. //(define: Secure for reboots)

if alt:radar < 100
{
	//WAIT for the GO!
	main_lifoff().
	
	CLEARSCREEN.
	DELETEPATH("FLIGHT_LOG.txt").
	LOG   "TIME,   VELO,   R:ALT,  Acel,   Q" to FLIGHT_LOG.txt.

	set TakeOffTime to TIME:SECONDS.
	set value1 to 0.
	set last_value1 to 0.

	update_phase_title("[1] LIFTOFF...",0, false).
	PRINT "Q-Max" 				at (0,2).
	PRINT "Dynamic Pressure" 	at (0,3).
	PRINT "q/Qmax" 				at (0,4).
	
	// (not used for falcons)
	// WAIT until alt:radar-30 > 50.
	// GEAR OFF.

	// LOOP1: UNTIL HALF Qmax
	until q > Qmax*.50
	{ 
		set thrust to 1.
		set H to altitude/(-5000).
		set p to p0*(e^H).
		// set r to altitude+BODY(DEFAULT_KSC):radius. //600000.
		// set g to GM/(r^2).
		//set Qmax to g/Cd.
		set q to .5*p*(verticalspeed^2). // pd = 1/2 ρ v^2
		
		PRINT ROUND(Qmax) at (22,2).
		PRINT ROUND(q) at (22,3).
		PRINT ROUND(q/Qmax*100,2)+ " %       " at (22,4).
		PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,6).
		
		set vsurf to velocity:surface.
		set Vsx to vsurf:x.
		set Vsy to vsurf:y.
		set Vsz to vsurf:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).	
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel, g).
	}.

	// Throttle is reduced to maintain a constant terminal velocity.
	update_phase_title("[2] LAUNCH-Trusting",1, false).
	set index2 to 6.
	PRINT "Throttle" at (0,1+index2).
	PRINT "Maxthrust: " at (0,2+index2).
	PRINT "delta: " at (0,3+index2).
	set x to 0.
	set phase to 0.
	until altitude > 30000 
	{
		if machVal > 0.8 and mphase = 0
		{
			update_phase_title("[3](Vehicle Transonic)", 1, false).
			set mphase to 1.
		}
		if machVal > 1.2 and mphase = 1
		{
			update_phase_title("[4](Vehicle SuperSonic)",1, false).
			set mphase to 2.
		}
		
		set H to altitude/(-5000).
		set I to altitude/(15000).
		if vehicle_type = "Falcon Heavy"
			set delta to (1*(e^I)*(-1))*1.2.
		else 
			set delta to (1*(e^I)*(-1)).
  
		set p to p0*(e^H).
		// set r to altitude+BODY(DEFAULT_KSC):radius. //600000.
		// set g to GM/(r^2).
		//set Qmax to g/Cd.
		
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
			set phase to 1.
		}
		
		if 	maxthrust > 0 {
			set Drag to ((Qmax*Cd*mass)/maxthrust).
			set Weight to (mass*g)/maxthrust.	
		} else {
			set Drag to ((Qmax*Cd*mass)/0.01). 
			set Weight to (mass*g)/0.01.		
		}
		//                    S   
		// NORTH ORINENTED:270 90                                                EST  NORTH DW
		//                    N                                                  WEST SOUTH UP 
		LOCK STEERING TO LOOKDIRUP(SHIP:SRFPROGRADE:VECTOR,SHIP:NORTH:VECTOR)+ Q(delta,0,180,0).
		
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
		
		PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,index2).
		PRINT ROUND(tThrust*100,1)+"%                " 	at (22,1+index2).	
		PRINT ROUND(Qmax)+"  " 							at (22,2).
		PRINT ROUND(q)+"  " 							at (22,3).
		PRINT ROUND(q/Qmax*100,2)+ " %       " 			at (22,4).
		PRINT ROUND(maxthrust)+ "     " 				at (22,2+index2).
		PRINT ROUND(delta)+ "    " 						at (22,3+index2).
	
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel, g).
	}.

	SAS OFF.
	CLEARSCREEN.
	PRINT " ".PRINT " ".
	update_phase_title("[5] ASCENT",1,false).
	//PRINT "Burn: from 30km to " + FINAL_ORBIT/1000 + "Km Apogee".
	set thrust to 1.
	set x to 0.
	RCS OFF.

	set Vs2 to 0.
	//LOOP3: After 30km, the effects of drag are minimal so thrust to 100% and Burn until MECO1:
	PRINT "mass: " 				at (0,4).
	PRINT "thrust: " 			at (0,5).
	PRINT "apoapsis: " 			at (0,6).
	PRINT "delta: " 			at (0,7).
	PRINT "VERTICALSPEED: " 	at (0,8).
	PRINT "mphase: " 			at (0,9).
	PRINT "deltaReduction: " 	at (0,10).
	until (Vs2 >= MECO1) //or (apoapsis >= FINAL_ORBIT)
	{ 
		set I to altitude/(15000).
		if vehicle_type = "Falcon Heavy"
		{
			set delta to (1*(e^I)*(-1))*1.2.
			if delta < (-60)
				set delta to (-60).
		} else {
			set delta to (1*(e^I)*(-1)).
			if delta < (-50)
				set delta to (-50).
		}

		SET steeringDir TO -90.	// W/E
		SET Vdeg to 90-delta+5.	// Vertical = 90 ---> SHOULD BE: 90-delta-5
		set Vroll to -270.		// Zero Rotation
		LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).

		set vorb to velocity:orbit.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).
		
		PRINT "Launch Site Distance: "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,3).
		PRINT ROUND(mass)+"    " 			at (22,4).
		PRINT thrust+"    " 				at (22,5).
		PRINT ROUND(apoapsis)+"    " 		at (22,6).
		PRINT ROUND(delta)+"    " 			at (22,7).
		PRINT ROUND(VERTICALSPEED)+"    " 	at (22,8).
		PRINT mphase 						at (22,9).
		PRINT deltaReduction 				at (22,10).
		
		if (Aceleration_value1 > 30)
		{
			if (mphase = 2) 
			{
				//PRINT "( Throttle down to avoid stress on vehicle )" at (0,11).
				set mphase to 3. // Acceleration Relation... to avoid stress on vehicle.
			}
			set thrust to (thrust-deltaReduction).
		}
		
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel, g).
	}.		

	RCS ON.
	SAS OFF.
	set thrust to 0.01.
	WAIT 1.
}

if altitude*1.1 < FINAL_ORBIT2
{

	CLEARSCREEN.
	PRINT " ".PRINT " ".
	update_phase_title("[6] STAGING...",1,false).
	PRINT "1st ORBIT: "+FINAL_ORBIT+"km, Done!".

	if vehicle_type = "Falcon Heavy" 
	{
		//SEND PROCESSOR ID TO BOOSTER --------------------------------------------
		IF PROCESSOR_STAGE1L:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
		{
		  PRINT "PROCESSOR_STAGE1: Message sent!".
		  WAIT 1.
		}
		//SEND PROCESSOR ID TO BOOSTER --------------------------------------------
		IF PROCESSOR_STAGE1R:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
		{
		  PRINT "PROCESSOR_STAGE1: Message sent!".
		  WAIT 1.
		}
	} else {
		//SEND PROCESSOR ID TO BOOSTER --------------------------------------------
		IF PROCESSOR_STAGE1:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
		{
		  PRINT "PROCESSOR_STAGE1: Message sent!".
		  WAIT 1.
		}	
	}

	RCS ON.
	if vehicle_type = "Falcon Heavy" 
	{
		AG6 ON. //Toggle: FH Boosters separator
		WAIT 0.5.
		if (KUniverse:ActiveVessel = SHIP) STAGE.
		WAIT 5.
	} else {
		main_stage().
	}

	// Stage-2 Initial Slow Burn:
	update_phase_title("[7] PHASE I OPT",1,false).
	UNLOCK STEERING.
	set thrust to 0.25.
	if vehicle_type = "Crew Dragon 2" or vehicle_type = "Falcon Heavy"
		WAIT 1.
	else
		WAIT 3.

	SAS OFF.

	if vehicle_type = "Crew Dragon 2"
	{
		SET Vdeg to 90-74.	// Vertical = 90
	}
	else
	if vehicle_type = "Falcon Heavy"
	{
		SET Vdeg to 90-85.	// Vertical = 90
	}else
	{	
		SET Vdeg to 90-81.	// Vertical = 90
	}

	SET steeringDir TO 90.	// W/E
	set Vroll to -270.		// Zero Rotation
	LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).

	function update_orbit_status 
	{
		PRINT "Launch Site Distance:   "+ROUND(VESSEL("Landingzone1"):GEOPOSITION:DISTANCE/1000,3)+" km   " at (0,9).
		PRINT "Periapsis: " + ROUND(periapsis/1000,1)+" km    " at (0,10).
		PRINT "Apoapsis: " + ROUND(apoapsis/1000,1)+" km    " at (0,11).
		PRINT "Eta:apoapsis: " + ROUND(eta:apoapsis,1)+" s    " at (0,12).
		PRINT "Eccentricity: " + ROUND(SHIP:ORBIT:ECCENTRICITY,1)+"     " at (0,13).
		PRINT "Altitude: " + ROUND(Altitude,1)+" km    " at (0,14).
		PRINT "Phase: " + phase at (0,15).
		PRINT "mphase: " + mphase at (0,16).
		PRINT "deltaReduction: " + deltaReduction at (0,17).
	}

	if vehicle_type = "Falcon Heavy"
		engines_thrustlimit_to (100).	//Main Core back to 100% now

	set thrust to 1.
	RCS OFF.
	set phase to 0.
	set deltaReduction to 0.
	UNTIL periapsis > 0 and apoapsis < FINAL_ORBIT2
	{
		LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).
		
		set eta_apoapsis to eta:apoapsis.
		
		set vorb to velocity:orbit.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set velocity_orbit to (Vsx^2)+(Vsy^2)+(Vsz^2).
		set Vs2 to (velocity_orbit).	//km/h
		
		
		if vehicle_type = "Falcon Heavy" and phase=0 and (Vs2 > MECO2 or altitude > FAIRSEP)
		{
			// FH:CORE Booster SEP:
			RCS ON.
			set thrust to 0.
			WAIT 1.
			if (KUniverse:ActiveVessel = SHIP) STAGE.
			WAIT 3.
			if (KUniverse:ActiveVessel = SHIP) STAGE.
			set phase to 1.
			set thrust to 1.
			RCS OFF.
			if vehicle_type = "Falcon Heavy"
				SET Vdeg to 90-80.5.
			WAIT 1.
		}
		if vehicle_type <> "Falcon Heavy" and altitude > FAIRSEP and phase = 0
		{
			update_phase_title("[8] FAIRING SEPARATION",1,false).
			if (KUniverse:ActiveVessel = SHIP) STAGE.
			set phase to 2.
		}
		
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
		
		update_orbit_status.
		
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel, g).
		
		set dx to eta:apoapsis.
		if dx > eta_apoapsis and stage:number < 7
			break.
	}


	update_phase_title("[7] PHASE II OPT",1,false).
	UNTIL (apoapsis >= FINAL_ORBIT2) 
	{
		LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).
		
		set vorb to velocity:orbit.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set velocity_orbit to (Vsx^2)+(Vsy^2)+(Vsz^2).
		set Vs2 to (velocity_orbit). //km/h
		
		// FH:CORE Booster SEP:
		if vehicle_type = "Falcon Heavy" and phase=0 and (Vs2 > MECO2 or altitude > FAIRSEP)
		{
			RCS ON.
			set thrust to 0.
			WAIT 1.
			if (KUniverse:ActiveVessel = SHIP) STAGE.
			WAIT 3.
			if (KUniverse:ActiveVessel = SHIP) STAGE.
			set phase to 1.
			set thrust to 1.
			RCS OFF.
			if vehicle_type = "Falcon Heavy"
				SET Vdeg to 90-75.
			else
				SET Vdeg to 90-85.
			WAIT 1.
		}
		
		if altitude > FAIRSEP and phase = 0
		{
			update_phase_title("[8] FAIRING SEPARATION",1,false).
			if (KUniverse:ActiveVessel = SHIP) STAGE.
			set phase to 2.
		}
		
		update_orbit_status.
		
		set vel to SQRT(Vs2).
		update_atmosphere (altitude, vel).
		log_data (vel, g).
	}

	UNLOCK STEERING.
	set thrust to 0.
	PRINT "Orbit insertion completed".
	WAIT 5.

	if orbit_type = "GSO"
	{
		//25% now on.
		LIST ENGINES IN myVariable.
		FOR eng IN myVariable {
			set eng:THRUSTLIMIT to 25.
			set eng:Gimbal:LIMIT to 25.
		}.
	}
}

set warp to 0.
wait 2.