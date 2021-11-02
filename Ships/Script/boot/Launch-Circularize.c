// --------------------------------------------------------------------------------------------
// Filename: Launch-Circularize.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Circularize an orbit after Launch-Orbit.c
// 01/Nov/2021
// --------------------------------------------------------------------------------------------

parameter FINAL_ORBIT. //Sample: 125000 or 150000 or 500000

function do_circle_step
{
	// Burn to circularize, theta is used to maintain the apogee in-front of the craft
	set y to .5.
	set Vo to 0.
	set z to 0.
	set x to 1. //x=Throttle.

	RCS ON.

	//SETUP:
	// SET steeringDir TO 90.		// W/E
	// SET steeringVdeg to 0.			// UP/DOWN: Vertical = 90
	// set steeringVroll to -270.			// Zero Rotation
	// LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//LOCK STEERING TO heading (90, PlanetOuter*theta).
	
	until (periapsis >= FINAL_ORBIT)
	{
		set last_ecc to SHIP:ORBIT:ECCENTRICITY.
		
		set previous_ratio to (periapsis/apoapsis).
		set thrust to x.
		set vorbit to velocity:orbit.
		set Vox to vorbit:x.
		set Voy to vorbit:y.
		set Voz to vorbit:z.
		set Vo to ((Vox^2)+(Voy^2)+(Voz^2))^.5.
		
		set r to max(apoapsis,periapsis)+BODY:radius. //apoapsis+BODY:radius. BODY("Kerbin")=600000.
		set vcir to (GM/r)^.5.				// Calculate circular velocity (vcir) at apoapsis altitude:
		set r to FINAL_ORBIT+BODY:radius. 	//apoapsis+BODY:radius. BODY("Kerbin")=600000.
		set per to periapsis+BODY:radius. 	//periapsis+BODY:radius. BODY("Kerbin")=600000.
		set a to (r+per)/2.
		set e to (r-per)/(r+per).
		set h to (GM*a*(1-(e^2)))^.5.
		set Va to h/r.
		set ar to (Va^2)/r.
		set g to GM/(r)^2.
		set W to mass*(g-ar).
		
		if y = .5 {
			set err to .75.
			set error to 1-(err*verticalspeed).
			if maxthrust > 0
				set theta to arcsin(W/maxthrust).
			else
				set theta to 0. //arcsin(W/0.1).
			set theta to theta*error.
		}.
		if stage:liquidfuel = 0 and z < 1
		{
			set z to 1.5.
		}.
		if (Vcir-Vo) < 100  and y < 1{
			set err to 2.5.
			set A to 10.
			set y to y+1.
		}.
		if (Vcir-Vo) < 10 and y < 2{
			set err to 5.
			set A to 1.
			set y to y+1.
		}.
		
		//In theory we should have it...
		if throttle > 0 and maxthrust = 0 
		{
			if vehicle_type <> "Space4"
				stage. //confirm_stage().
			else
				AG9 ON.
			WAIT 1.
		}.
		
		if y > 1 
		{
			set error to 1-(err*verticalspeed).
			set C to mass*A.
			set B to ((W^2)+(C^2))^.5.
			set x to 2*B/maxthrust.
			
			if maxthrust > 0
				set x to 2*B/maxthrust.
			else
				set x to (2*B/0.1).
			
			if vehicle_type = "Space4"
			{
				if x > 0.5.
					set x to 0.5.	//Slow part...
			} else {
				if vehicle_type <> "Crew Dragon 2"		
				{
					if x > 1
						set x to 1.
				}else{
					if x > 0.5.
						set x to 0.5.	//Slow part...
				}
			}
			
			set theta to arctan(W/C).
			set theta to theta*error.
		}.

		PRINT ROUND(verticalspeed,2)	+ " m/s     " 	at (20,2).
		PRINT ROUND(Vo,2)				+ " m/s     " 	at (20,3).
		PRINT ROUND(vcir) 		+ " m/s     " 		at (20,4).
		PRINT ROUND(theta,2)      + "     " 			at (20,5).
		PRINT ROUND(vcir-Vo,2)	+ "     " 			at (20,6).
		PRINT y 					+ "     " 			at (20,7).
		PRINT ROUND(x*100,1)		+ " %   " 			at (20,8).
		PRINT ROUND (vcir-Vo)     + " m/s     "		at (20,10).
		PRINT ROUND(SHIP:ORBIT:ECCENTRICITY,3)+"   " at (20,11).
		
		set vel to (Vo).
		update_atmosphere (altitude, vel).
		log_data (vel).
		
		SET steeringDir TO 90.		// W/E
		if verticalspeed < 0 
			SET steeringVdeg to (-verticalspeed/8).	// UP/DOWN: Vertical = 90 (magical number: 8 tune precision)
		else
			SET steeringVdeg to 0.			// UP/DOWN: Vertical = 90
		set steeringVroll to -270.			// Zero Rotation
		
		update_steering_status().
		LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).	//LOCK STEERING TO heading (90, PlanetOuter*theta).

		wait 0.010.
		if SHIP:ORBIT:ECCENTRICITY > last_ecc and SHIP:ORBIT:ECCENTRICITY < 0.1 and periapsis > BODY:ATM:HEIGHT
		{
			LOCK THROTTLE TO 0.
			break.
		}
	}
	LOCK THROTTLE TO 0.
}

// SETUP CIRCULARIZATION:
// --------------------------------------------------------------------------------------------
set x to 1.
set circle_error to 0.
set GM to BODY:mu.
set v to 0.

CLEARSCREEN. print " ".print " ".
update_phase_title("WAIT TO CIRCULARIZE",1, false).
if KUniverse:ActiveVessel = SHIP
	SET MAPVIEW TO TRUE.	// map view on
	
until (altitude > BODY:ATM:HEIGHT) or (VERTICALSPEED < 0) //BODY:ATM:HEIGHT=140000m (EARTH)
{
	WAIT 0.1.
	PRINT_STATUS (3).
}
	
SAS OFF. //Hands off..
set kuniverse:timewarp:MODE to "RAILS".	wait 1.
set warp to  0.

// WAIT FOR PE/AP:
// --------------------------------------------------------------------------------------------
if KUniverse:ActiveVessel <> SHIP {
	update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
	UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
}
if periapsis > BODY:atm:height and verticalspeed < 0 //means moving to PE and not to AP
{
	set warp to 2.
	wait until verticalspeed > 0.
}
if ship:verticalspeed > 0 and eta:apoapsis > 60		//Move closer to AP
{
	if vehicle_type = "Space4"
		warp_until_apoapsis(30).
	else
		warp_until_apoapsis(60).
}

// FINAL COUNT DOWN:
// --------------------------------------------------------------------------------------------
if vehicle_type <> "SaturnV"
{
	prograde_check().
	WAIT 1.
}

CLEARSCREEN.
update_phase_title("Circularize: "+FINAL_ORBIT,1, false).
PRINT "Vertical Speed" 	at (0,2).
PRINT "Orbital Speed" 	at (0,3).
PRINT "Target (Vcir)" 	at (0,4).
PRINT "Theta" 			at (0,5).
PRINT "vcir-Vo: " 		at (0,6).
PRINT "Y: "       		at (0,7).
PRINT "thrust(x): "     at (0,8).
PRINT "vcir-Vo: " 		at (0,10).
PRINT "Eccentricity: " 	at (0,11).
do_circle_step().

// ITS DONE:
// --------------------------------------------------------------------------------------------
// SAS OFF. wait 0.1.
// LOCK STEERING TO PROGRADE. wait 0.1.
//wait 5.
RCS OFF.

CLEARSCREEN.
update_phase_title("C-In Parking Orbit", 1, true).
set e to (apoapsis-periapsis)/(apoapsis+periapsis).
PRINT "Eccentricity Error: " at (0,2). PRINT ROUND(e,2) at (20,2).
WAIT 5.

SET MAPVIEW TO FALSE.  	// map view off
lock throttle to 0.		//set thrust to 0.
LOCK STEERING TO SHIP:PROGRADE.//  + R(0,0,180).


//Kill KSP spin bugs, of a out of focus vessel:
set kuniverse:timewarp:MODE to "RAILS".	wait 1. //RESET
set warp to  0. wait 0.1.

RCS OFF. wait 0.1.