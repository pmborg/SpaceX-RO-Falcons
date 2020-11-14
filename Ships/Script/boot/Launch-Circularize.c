// --------------------------------------------------------------------------------------------
// Filename: Launch-Circularize.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Circularize an orbit after Launch-Orbit.c
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

parameter FINAL_ORBIT. //Sample: 125000 or 150000 or 500000

PRINT "[5]Circularize------------------------------------".
SET MAPVIEW TO TRUE.	// map view on
SAS OFF.

//Calculate circular velocity at apoapsis altitude:
set x to 1.
set GM to BODY:mu. 		//GM = 
set r to max(apoapsis,periapsis)+BODY:radius. //apoapsis+BODY:radius. BODY("Kerbin")=600000.
set vcir to (GM/r)^.5.
set r to FINAL_ORBIT+BODY:radius. //apoapsis+BODY:radius. BODY("Kerbin")=600000.
set v to 0.
set per to periapsis+BODY:radius. //periapsis+BODY:radius. BODY("Kerbin")=600000.
set a to (r+per)/2.
set e to (r-per)/(r+per).
set h to (GM*a*(1-(e^2)))^.5.
set Va to h/r.
set ar to (Va^2)/r.
set g to GM/(r)^2.
set W to mass*(g-ar).

if maxthrust > 0
	set theta to arcsin(W/maxthrust).
else
	set theta to 0. //arcsin(W/0.01).

//LOOP5: Waiting on apoapsis arrival.
CLEARSCREEN.
PRINT "Warp to Apogee".
PRINT "theta: " + ROUND(theta,2).

function wait_for_AP
{
	parameter w.
	
	set warp to 0.
	wait 1.
	WAIT until eta:apoapsis < 3000.
	set warp to 2.
	WAIT until eta:apoapsis < 1000.
	set warp to 1.
	WAIT until eta:apoapsis < w.
	set warp to 0.
}


function do_circle_step
{
	CLEARSCREEN.
	// Burn to circularize, theta is used to maintain the apogee in-front of the craft
	set y to .5.
	set Vo to 0.
	set z to 0.
	set x to 1. //x=Throttle.

	// if vehicle_type = "Crew Dragon 2"		
		RCS ON.

	PRINT "vcir-Vo: "+ (vcir-Vo) at (0,10).

	//LOOP6: 
	until (vcir-Vo < .001 and periapsis >= FINAL_ORBIT) 
	{
		//set last_vcir_Vo to vcir-Vo.
		
		set previous_ratio to (periapsis/apoapsis).
		set thrust to x.
		set vorbit to velocity:orbit.
		set Vox to vorbit:x.
		set Voy to vorbit:y.
		set Voz to vorbit:z.
		set Vo to ((Vox^2)+(Voy^2)+(Voz^2))^.5.
		set ar to (Vo^2)/r.
		set W to mass*(g-ar).

		PRINT "[6]Circularize-Burn to Circularize Orbit:" at (0,0).
		PRINT "Vertical Speed" at (0,1).
		PRINT "Orbital Speed" at (0,2).
		PRINT "Vcir" at (0,3).
		PRINT ROUND(vcir) at (20,3).
		PRINT "Theta" at (0,4).
		PRINT ROUND(theta,3) at (20,4).
		PRINT "vcir-Vo: " at (0,5).
		PRINT "Y: "       at (0,6).
		PRINT "x: "       at (0,7).

		if (periapsis/apoapsis)>0 and (apoapsis/periapsis) < 1.1 and apoapsis>FINAL_ORBIT and periapsis > max(1.5*body:atm:height,40000) //and Y > 3
		{
			LOCK THROTTLE TO 0.
			// PRINT "break." at (0,9).
			// PRINT "WAIT 15.".
			// WAIT 15.
			break.
		}
		
		if (periapsis/apoapsis)>0 and (periapsis/apoapsis) > 1.1 and apoapsis>FINAL_ORBIT and periapsis > max(body:atm:height,40000) //and Y > 4
		{
			LOCK THROTTLE TO 0.
			// PRINT "break." at (0,9).
			// PRINT "WAIT 10.".
			// WAIT 10.
			break.
		}

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
		// if (Vcir-Vo) < 1 and y < 3{
			// set err to 8.
			// set A to .1.
			// set y to y+1.
		// }.
		
		//In theory we should have it...
		if throttle > 0 and maxthrust = 0 
		{
			CLEARSCREEN.
			confirm_stage().
			WAIT 2.
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
			
			if vehicle_type <> "Crew Dragon 2"		
			{
				if x > 1
					set x to 1.
			}else{
				if x > 0.5.
					set x to 0.5.
			}
			
			set theta to arctan(W/C).
			set theta to theta*error.
		}.

		PRINT ROUND(verticalspeed,2)	+ "     " at (20,1).
		PRINT ROUND(Vo,2)				+ "     " at (20,2).
		PRINT ROUND(theta,3)			+ "     " at (20,4).
		PRINT ROUND(vcir-Vo,2)		+ "     " at (20,5).
		PRINT y 					+ "     " at (20,6).
		PRINT x 					+ "     " at (20,7).
		PRINT "apoapsis/periapsis :"+ (apoapsis/periapsis)	at (0,8).
		
		// if vcir-Vo > last_vcir_Vo
			// break.
		//WAIT 0.1.
	}.
}

//LOCK STEERING TO heading (90, PlanetOuter*theta).
SET steeringDir TO 90.	// W/E
SET Vdeg to 0.			// Vertical = 90
set Vroll to -270.		// Zero Rotation
LOCK STEERING TO HEADING(steeringDir,Vdeg,Vroll).


if ship:verticalspeed > 0
	wait_for_AP(30).

do_circle_step().

if orbit_type = "GSO"
{
	set FINAL_ORBIT to 35786000. //35,786 km
	wait_for_AP(30).
	do_circle_step().
}


CLEARSCREEN.
PRINT "[7] DONE: Craft is now in Parking Orbit ---".

set e to (apoapsis-periapsis)/(apoapsis+periapsis).
PRINT "Eccentricity" at (0,1). PRINT ROUND(e) at (20,1).
set avg to (apoapsis+periapsis)/2-FINAL_ORBIT.
set error to avg/FINAL_ORBIT*100.
PRINT "Error " + ROUND(error) + "%" at (0,2).
PRINT "Craft is now in Parking Orbit -> Begin Phase I" at (0,3).
WAIT 5.

RCS OFF.
SET MAPVIEW TO FALSE.  	// map view off
lock throttle to 0.		//set thrust to 0.
SAS ON.
WAIT 1.
set sasmode TO "PROGRADE".