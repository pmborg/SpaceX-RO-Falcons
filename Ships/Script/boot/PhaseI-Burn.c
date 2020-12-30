// --------------------------------------------------------------------------------------------
// Filename: PhaseI-Burn.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to todo the Manouver in prograde to the: mission_target
// 28/Nov/2020
// --------------------------------------------------------------------------------------------

parameter goto_mission_target.

set originBody to BODY.
set thrust to 0.
set warp to 0.
lock steering to prograde.
//lock steering to up + R(0,-90,180).
lock throttle to thrust.
wait 1.

//-------------------------------------------------------------------------------
// KERBIN:
set GM to originBody:mu. 			//GM = 3.5316*(10^12). for Kerbin
set target to goto_mission_target.	//"Mun"/"Moon", etc
runpath("boot/lib_trajectory.c").
set warp to 3. wait 1.
get_rendevous_nodes().
set warp to 0. wait 1.
set target to goto_mission_target.	//"Mun"/"Moon", etc


//-------------------------------------------------------------------------------

// Calculate the required Delta-V.
// The Insertion burn distance and Final Apoapsis for this burn were calculated on MatLab.
// The Calculated Final Periapsis on the Mun is around 50km.

// Calculations: 
// https://gaming.stackexchange.com/questions/144030/how-do-i-calculate-necessary-delta-v-for-circularization-burn
// dV = SQRT ( GM . (2/r - 1/a ) )
//
// r is the distance from the center of the planet to the orbiting vessel at that point in the orbit (i.e. the vessel's altitude plus the radius of the planet). 
// a is the semi-major axis of the orbit (the average of the apoapsis and periapsis altitudes, plus the radius of the planet).

// https://en.wikipedia.org/wiki/Vis-viva_equation
// dV = SQRT ( GM . (2/r - 1/a ) )
//
//
//Where:
//
//    v is the relative speed of the two bodies
//    r is the distance between the two bodies
//    a is the length of the semi-major axis (a > 0 for ellipses, a = ∞ or 1/a = 0 for parabolas, and a < 0 for hyperbolas)
//    G is the gravitational constant
//    M is the mass of the central body

// https://en.wikipedia.org/wiki/Hohmann_transfer_orbit
// https://github.com/yehoodig/kos-missions/tree/master/lib

//Rp + Ra = 2a
//Ra = a(1-e)
//Rp = a(1+e)

//Semi-major axis 	12 000 000 m
//Apoapsis 			12 000 000 m
//Periapsis 		12 000 000 m

//set Ra to target:distance + BODY("Kerbin"):radius.	//[set Ra to 1.2*(10^7).] //12 000 000 (m) Dist from Kerbin to Mun
//Apoapsis+Periapsis/2 = 6 000 000 + Semi-major axis /2	// https://wiki.kerbalspaceprogram.com/wiki/Mun
set Ra to ( BODY(goto_mission_target):Apoapsis + BODY(goto_mission_target):periapsis ) /2. //1.2*(10^7).
set Rp to (apoapsis+periapsis)/2+ BODY(goto_mission_target):radius + altitude.	//[set Rp to (apoapsis+periapsis)/2+600000.]  //Raio equatorial Kerbin: 600 000 m 

// Minmus: 61301
set e to (Ra-Rp)/(Ra+Rp).
set a to (Ra+Rp)/2.
set h to (GM*a*(1-(e^2)))^.5. // "^.5" = SQRT

//Rp.Vp = Ra.Va
set Vp to h/Rp. //perigee 
set Va to h/Ra. //apogee

// GM / r = v^2, r = (R+h), v = Vcir
set Vcir to (GM/Rp)^.5.
set dV to Vp-Vcir.

//set LAUNCH to 1.229146628119515*(10^7). //MAGIC NUMBER
set LAUNCH to Ra.

print "-----------------".
print "Ra: "+Ra.
print "Rp: "+Rp.
print "GM: "+GM.
print "e: "+e.
print "a: "+a.
print "h: "+h.
print "Vp: "+Vp.
print "Va: "+Va.
print "VCir: "+Va.
print "dV: "+Va.
print "target: "+target.
print "LAUNCH: "+LAUNCH.

//-------------------------------------------------------------------------------
{
	clearscreen.
	print "PhaseI-Rotate to "+goto_mission_target+" Burn" at (0,0).
	RCS ON.
	wait 15.
	RCS OFF.
}.

//-------------------------------------------------------------------------------
clearscreen.
RCS OFF.
print "[PhaseI-Burn] to escaping "+body:name at (0,0).

set thrust to 1.
if vehicle_type = 3 
	{AG2 ON. AG1 OFF. AG1 ON. wait 1.}
	
print "[PhaseI-Burn] to "+goto_mission_target+" Intercept: "+goto_mission_target at (0,0).
print "Orbital Speed(V)" at (0,1).
print "Desired Speed (Vp)" at (0,2). 
print "Current Apoapsis*" at (0,3).
print "Desired Apoapsis" at (0,4). print ROUND(Ra - BODY(goto_mission_target):RADIUS) at (25,4).
print "Target [(Vp-V) < 0]:" at (0,5).
print "Vo: (phase) " 		 at (0,6). 		
print "Y: (phase) " 		 at (0,7). 		
print "X: (power)" 			 at (0,8).

set x to 1.		//thrust
set y to .5.	//Burn-Phase
set V to 0.		//Initial relative speed.
set vec to velocity:orbit.
set Vx to vec:x.
set Vy to vec:y.
set Vz to vec:z.
set Vo to ((Vx^2)+(Vy^2)+(Vz^2))^.5. //Original Speed.
set Vp to Vo + burn_dV.
print  ROUND(Vp) at (25,2).
	
set HaveEncounter to False.
until (Vp-V) < 0.001 {
	set vec to velocity:orbit.
	set Vx to vec:x.
	set Vy to vec:y.
	set Vz to vec:z.
	set V to ((Vx^2)+(Vy^2)+(Vz^2))^.5.

	if phaseAngle = 999 {
		if (Vp-V) > 100  and y < 1{
			set x to 1.	//set x to 0.75.	//POWER! (No need max)
			set y to y+1.
		}
		
		if (Vp-V) < 150  and y < 2 {
			set y to y+1.
			
			if (maxthrust > 4000) {
				set x to (mass*5)/maxthrust.
			} else
				set x to (mass*10)/maxthrust.
		}
		if (Vp-V) < 20 and y < 3 {
			set x to (mass*5)/maxthrust.
			set y to y+1.
		}
		if (Vp-V) < 2 and y < 4 {
			set x to (mass*5)/maxthrust.
			set y to y+1.
		}
	}

	set thrust to x.
	lock throttle to thrust. //wait 1.
	
	if maxthrust = 0 and vehicle_type < 3 {
		confirm_stage(). wait 5. // Decouple
	}
	
	// if apoapsis > originBody:SOIRADIUS + 1000000
		// if ship:Orbit:TRANSITION = "ENCOUNTER" 
			// break.

	// |ship:patches|
	if ship:patches:length > 1
	{
		set thesepatches to ship:patches.
		print "ship:patches[1]:apoapsis= "+ ROUND(thesepatches[1]:apoapsis) +"        " 	at (0,11).
		print "thesepatches= "+thesepatches+"        " 								at (0,12).

		//// if thesepatches[1]:apoapsis > (Ra + BODY(goto_mission_target):RADIUS)
		//	// break.
		// if thesepatches[1]:apoapsis >= (BODY(goto_mission_target):apoapsis) {
					// set thrust to 0.
					// PRINT ("####COND 1").
					// break.
		// }
		if ship:Orbit:TRANSITION <> "FINAL" 
		{
			FROM {local i is 0.} UNTIL i = thesepatches:length-1 STEP {SET i to i + 1.} DO 
			{
				print "thesepatches[i] "+thesepatches[i]+"        " at (0,14+i).
				if thesepatches[i]:name = goto_mission_target {
					set thrust to 0.
					PRINT ("####COND 2") at (0,20).
					set HaveEncounter to True.
					break.
				}
			}
		}
	}
	
	print ROUND(V)+"    " 		 at	(25,1).//Orbital Speed
	print ROUND(apoapsis)+"    " at	(25,3).//Current Apoapsis
	print ROUND(Vp-V)+"    " 	 at	(25,5).
	print ROUND(Vo)+"    "  	 at (25,6).
	print y 					 at (25,7).
	print x 					 at (25,8).
	print "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"     " at (0,10). //FINAL
	print "maxthrust: "+maxthrust+"     " 					 	  at (0,11).

	if apoapsis > (Ra - BODY(goto_mission_target):RADIUS)*0.7 {
		if (maxthrust > 5000) {
			//shutDownAllEngines().
			//activateLiquidEngine().
			set x to max(0.3,(mass*5)/maxthrust).
		}
	}

	if HaveEncounter {
		print "Encounter" at (0,21).
		break.
	}
	if body:name = goto_mission_target and apoapsis > (Ra - BODY(goto_mission_target):RADIUS) {
		print "#### break1" at (0,22).
		break.
	}
	//and body:name = goto_mission_target
	if (SHIP:PERIAPSIS < body:atm:height) {
		print "#### break2" at (0,23).
		break.
	}
}.

set thrust to 0.

// if vehicle_type = 3 
	// { AG2 ON. wait 1.}
//shutDownAllEngines().	
//SAS OFF.

print "BURN-I END!" at (0,25).
wait 1.
PRINT "Press [ENTER] to Confirm: Warp             "at (0,26)..set ch to terminal:input:getchar().
clearscreen.

REMOVE nd.  
REMOVE ned2.  

//-------------------------------------------------------------------------------
clearscreen.
print "PhaseI-Burn: Warp Out of Kerbin SOI".
set x to 0.
set warp to 6.
until x = 1 {
	
	if body:name <> DEFAULT_KSC {
		if warp < 6 {
			set warp to 0.
			set x to 1.
		}
	}
		
	wait 1.
	print "body: " + body:name at (0,11).
	print "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"     " at (0,10). //FINAL
}.

print "PhaseI-Burn: ENDED!".