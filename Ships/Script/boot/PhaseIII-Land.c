// --------------------------------------------------------------------------------------------
// Filename: PhaseIII-Land.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://github.com/AlexAscherson/Kerbal-Kos-Programming/blob/master/old/r1.ks
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              	- Used to land any where near by, (sample: after de-orbit on Mun or Moon)
//                    used in missions (Beyond Earth)
// 29/Nov/2021
// --------------------------------------------------------------------------------------------

CLEARSCREEN.
update_phase_title("PHASEIII-DESCENDING", 1).

set GM to mission_target:mu. 		//GM = 6.5138398*(10^10).  (MUN)

set thrust to 0.
lock throttle to thrust.

LIGHTS ON. // lights off
if vehicle_type <> "Space4"
	RCS ON.
else
	RCS OFF.
PRINT "@>5000km   " at (20,1).
if alt:radar > 5000000 //5000km
{
	WAIT 1.
	set warp to 6.
	WAIT until alt:radar < 5000000.
	set warp to 0.
	WAIT 1.
}

PRINT "@>1000km   " at (20,1).
if alt:radar > 1000000 //1000km
{
	WAIT 1.
	set warp to 5.
	WAIT until alt:radar < 1000000.
	set warp to 0.
	WAIT 1.
}

//// ALT: 1000Km -> Max-Speed: 1000m/s
set max to alt:radar / 1000.
PRINT "max speed: " + ROUND(max) at (0,5).
PRINT "@<1000km   " at (20,1).
until alt:radar < 1000000 
{
	set r to altitude+1000000.
	set g to GM/(r)^2.
	set Vdown to (-1)*verticalspeed.
	set err to 1.	
	set error to err*(Vdown-max).
	
	PRINT "Vertical Vector Speed(Vdown): " + ROUND(Vdown) at (0,2).
	if max+10 < Vdown {
		if maxthrust > 0
			set thrust to error*((1000*SHIP:MASS*g)/maxthrust).
	}
	else {
		set thrust to 0.
		break.
	}.
}

set warp to 0. 
set max to 700.
PRINT "@>100km   " at (20,1).
UNLOCK STEERING.
SET thrust TO 0.1.
if altitude > 100000 //100km
{
	lock steering to retrograde.
	
	set warp to 3.
	set Vo to 0.
	until (Vo > 700 or altitude < 100000) {
		set vorbit to velocity:orbit.
		set Vox to vorbit:x.
		set Voy to vorbit:y.
		set Voz to vorbit:z.
		set Vo to ((Vox^2)+(Voy^2)+(Voz^2))^.5.
		WAIT 1.
	}
	
	set warp to 0. WAIT 1.
}

if vehicle_sub_type <> "SN20-Profile"
	SAS OFF.
if vehicle_type <> "Space4"
	RCS ON.
set breakspeed to 1000.
PRINT "@<100km   " at (20,1).

SET x to 0.0.
set err to 5.
set t to 0.
SET Vdown TO 1.
lock steering to retrograde. 
set error to 1.

PRINT "Vertical Vector Speed(Vdown): " at (0,3).
PRINT "alt:radar: " at (0,4).
PRINT "Next Max-Speed: " at (0,5).
//PRINT "throttle: " at (0,6).
PRINT "error: " at (0,6).
PRINT "err: " at (0,7).
PRINT "g: " at (0,8).
PRINT "mass: " at (0,9).
PRINT "thrust: " at (0,10).
PRINT "maxthrust: " at (0,11).
PRINT "t: " at (0,12).
PRINT "x: " at (0,13).

until Vdown < 1 and alt:radar < 100
{
	if x = 0
	{
		if alt:radar < 100000 	//<100km
			set max to 500.
		if alt:radar < 85000 and x = 0	//<060km
		{
			set max to 400.	
			UNLOCK STEERING.
			RCS ON.
			sas on. wait 1.
			set sasmode TO "retrograde". wait 1.
			set x to 0.5.
		}
		if alt:radar < 55000	//<030km
			set max to 300.
		if alt:radar < 45000
			set max to 200.
		if vehicle_type = "Space4" and maxthrust = 0
			AG2 ON.
	}

	if alt:radar < 8000
		set max to 100.
	if alt:radar < 4000 {
		set max to 50.
		set err to 1.
	}
	if alt:radar < 1000
		set max to 20.
	if alt:radar < 500
		set max to 6.
	if alt:radar < 500
		GEAR ON.	// retract gear
	if alt:radar < 300 {
		set max to 6.
	}
	if alt:radar < 200 {
		set max to 3.
	}
		
	if alt:radar < 7500 and x<1
	{
		if vehicle_sub_type <> "SN20-Profile" and vehicle_company = "SpaceX" 
			activateOneEngine().
		if vehicle_type = "Space4"
		{
			RCS ON.
			SAS ON.
			set thrust to 1.
			until (Vdown < 10)  {
				set Vdown to (-1)*verticalspeed.
				WAIT 0.1.
			}

			update_phase_title("ACTIVATE VERTICAL", 1, false).
			if  maxthrust > 0
				{ AG2 ON. wait 0.5.}
			if  maxthrust > 0
				{ AG2 OFF. wait 0.5.}			
			AG3 ON. wait 1.
			RCS ON. wait 1.
			SAS ON. wait 1.
			AG8 ON. wait 1.
		}
		set x to 1.
	}
	
	if (alt:radar > 5000) 
	{
		set vorbit to ship:velocity:surface. //Surface!!
		set Vox to vorbit:x.
		set Voy to vorbit:y.
		set Voz to vorbit:z.
		set Vdown to ((Vox^2)+(Voy^2)+(Voz^2))^.5.
	} else
		set Vdown to (-1)*verticalspeed.
	
	if alt:radar < 200 
	{
		if x < 2
		{
			set x to 2.
			if KUniverse:ActiveVessel = SHIP and vehicle_sub_type <> "SN20-Profile" and vehicle_company = "SpaceX" 
				set TARGET to LandingZone.
			unlock steering. 
			SAS ON.
			RCS ON.
			if vehicle_sub_type = "SN20-Profile" or vehicle_type = "Space4"
			{
				set sasmode to "STABILITY".
			}
			else
			{
				SET MAPVIEW TO FALSE. wait 0.1. 
				set sasmode TO "RETROGRADE". wait 0.1. 
			}
		}
		if vehicle_type = "SN16-Profile1"
			SET err TO 0.50.

		if vehicle_type = "Space4"
			SET err TO 0.95.
		else
			SET err TO 0.80.
	}

	if alt:radar < 125 {
		// if vehicle_type = "SN16-Profile1"
			// set max to 1. 	//6m/s
		// else
			// set max to 0.5. //m/s
		
		set max to 0.75.
		set error to err*((Vdown+error/2)-max).
	}
	else
		set error to err*(Vdown-max).

	if error <= 0
		set error to 4.
	
	set r to altitude + mission_target:radius.
	set g to GM/(r)^2.
	set max_t to maxthrust.
	if max_t > 0
		set t to (error/5)*((mass*g)/max_t).
	
	if t < 0.20 and vehicle_type <> "Space4"
		set thrust to 0.20.
	else
		set thrust to t.

	if  x < 2	
		PRINT "(mode:2)" at (0,2).
	else
		PRINT "(KISS mode)" at (0,2).

	PRINT ROUND(Vdown) +" m/s    " at (30,3).
	PRINT ROUND(alt:radar)+" m     " at (30,4).
	PRINT ROUND(max,1) + " m/s     " at (30,5).
	//PRINT ROUND(thrust) + "     " at (30,6).
	PRINT ROUND(error,1) + "     " at (30,6).
	PRINT ROUND(err,2) + "     " at (30,7).
	PRINT ROUND(g,2) + "     " at (30,8).
	PRINT ROUND(SHIP:MASS) + " t     " at (30,9).
	PRINT ROUND(thrust*100,1) + " %     " at (30,10).
	PRINT ROUND(maxthrust) + " kN     " at (30,11).
	PRINT ROUND(t,1) + "     " at (30,12).
	PRINT x + "     " at (30,13).
}

PRINT vehicle_type+" has: "+status at (0,25).
set thrust to 0.
unlock steering.
SAS ON.
RCS ON.
WAIT 10.
RCS OFF.
SAS OFF.
//BRAKES OFF.	// Air Breaks off
if vehicle_type = "Space4"
	BRAKES ON.
shutDownAllEngines().
PRINT "All engines, secure and shutdown" at (0,26).