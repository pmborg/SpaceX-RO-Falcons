// --------------------------------------------------------------------------------------------
// Filename: PhaseI-Burn.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to todo the Manouver in prograde to the: mission_target
// 06/Nov/2021
// --------------------------------------------------------------------------------------------

parameter goto_mission_target.

function DOTHEMAINBURN 
{

	set originBody to BODY.
	set thrust to 0.
	set warp to 0.
	lock steering to prograde. //lock steering to up + R(0,-90,180).
	lock throttle to thrust.
	wait 1.

	//-------------------------------------------------------------------------------
	// KERBIN:
	set GM to originBody:mu. 			//GM = 3.5316*(10^12). for Kerbin
	set target to goto_mission_target.	//"Mun"/"Moon", etc
	runpath("boot/lib_trajectory.c").
	set warp to 2. wait 1.
	set node to get_rendevous_nodes().	//CALCULATE Manouver
	set warp to 0. wait 1.
	set target to goto_mission_target.	//"Mun"/"Moon", etc

	if vehicle_type = "Space4"
	{
		UNLOCK STEERING. wait 0.1.
		SAS ON. wait 0.1.
		SET MAPVIEW TO FALSE. wait 2. set sasmode to "maneuver". wait 0.1.
	}

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

	set Ra to ( BODY(goto_mission_target):Apoapsis + BODY(goto_mission_target):periapsis ) /2.
	set Rp to (apoapsis+periapsis)/2+ BODY(goto_mission_target):radius + altitude.

	set e to (Ra-Rp)/(Ra+Rp).
	set a to (Ra+Rp)/2.
	set h to (GM*a*(1-(e^2)))^.5. // Note: "^.5" = SQRT

	//Rp.Vp = Ra.Va
	set Vp to h/Rp. //perigee 
	set Va to h/Ra. //apogee

	// GM / r = v^2, r = (R+h), v = Vcir
	set Vcir to (GM/Rp)^.5.
	set dV to Vp-Vcir.

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
	clearscreen.
	print " ".print " ".
	update_phase_title("PhaseI-Rotate", 0, false).
	print "PhaseI-Rotate to "+goto_mission_target+" Burn" at (0,2).
	
	prograde_check(). 						// WAIT WITH RCS FOR PROGRADE DIRECTION
	
	SET MAPVIEW TO FALSE. wait 2. set sasmode to "maneuver". wait 0.1.
	warp_until_node (node, 30).				//WARP to NODE

	//-------------------------------------------------------------------------------
	clearscreen.
	print " ".print " ".
	update_phase_title("[PhaseI-Burn]", 0, false).
	set thrust to 1.

	set py to 4.
	print "Escaping "+body:name at (0,py+0).
	print "To "+goto_mission_target+" Intercept: "+goto_mission_target at (0,py+1).

	//SETUP MAIN BURN:
	set x to 1.		//thrust
	set y to .5.	//Burn-Phase
	set V to 0.		//Initial relative speed.
	set Vo to velocity:orbit:mag.
	set Vp to Vo + burn_dV.
	set np to node:deltav. //points to node, don't care about the roll direction.
	
	//lock steering to np.
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS OFF. wait 0.1.

	//DO MAIN BURN:
	set HaveEncounter to False.
	set last_node_remaning_deltaV to 9999999999.
	until HaveEncounter //(Vp-V) < 0.001 
	{
		set V to velocity:orbit:mag.

		if phaseAngle = 999  // Moon or (Mun or Minimus)...
		{
			if (Vp-V) < 5 and y < 1 {
				set x to (mass*5)/maxthrust.
				set y to y+1.
			}
		}

		set thrust to x.
		lock throttle to thrust. //wait 1.
		
		if maxthrust = 0 
			{ confirm_stage(). wait 5. }
		
		if ship:patches:length > 1
		{
			if ship:Orbit:TRANSITION <> "FINAL" 
			{
				set thesepatches to ship:patches.
				FROM {local i is 0.} UNTIL i = thesepatches:length-1 STEP {SET i to i + 1.} DO 
				{
					print "thesepatches["+i+"] "+thesepatches[i]+"        " at (0,18+i).
				}
			}
			
		}
		
		print "Orbital Speed(V)" at (0,py+2).	print ROUND(V)+" m/s   " 		 at	(25,py+2).
		print "Current Apoapsis*" at (0,py+4).  print ROUND(apoapsis/1000)+" km    " at	(25,py+4).//Current Apoapsis
		print "Desired Apoapsis" at (0,py+5). 	print ROUND((Ra - BODY(goto_mission_target):RADIUS)/1000)+ " km   " at (25,py+5).
		print "Y: (phase) " 		 at (0,py+8). print y 					 at (25,py+8).
		print "X: (power)" 			 at (0,py+9). print x 					 at (25,py+9).
		set vorb to node:deltav.
		set Vsx to vorb:x.
		set Vsy to vorb:y.
		set Vsz to vorb:z.
		set Vs2 to (Vsx^2)+(Vsy^2)+(Vsz^2).
		set node_remaning_deltaV to SQRT(Vs2).
		print "node:deltav" 		 at (0,py+10). print ROUND (node_remaning_deltaV,1)+ " m/s     "		 at (25,py+10).
		print "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"     " at (0,24). //FINAL
		print "maxthrust: "+ROUND (maxthrust)+"     " 					 	  at (0,25).

		if (node_remaning_deltaV) > 100
			RCS OFF.
		else
			RCS ON.

		if node_remaning_deltaV > last_node_remaning_deltaV and ship:Orbit:TRANSITION = "ESCAPE" 
		{
			print "#### break using deltaV" at (0,20+5).
			break.
		}
	    set last_node_remaning_deltaV to node_remaning_deltaV.

		if apoapsis > (Ra - BODY(goto_mission_target):RADIUS)*0.7 {
			if (maxthrust > 5000)
				set x to max(0.3,(mass*5)/maxthrust).
		}

		if body:name = goto_mission_target and apoapsis > (Ra - BODY(goto_mission_target):RADIUS) {
			print "#### break 1" at (0,22+5).
			set HaveEncounter to True.
			break.
		}

		if (SHIP:PERIAPSIS < body:atm:height) {
			print "#### break 2" at (0,23+5).
			set HaveEncounter to True.
			break.
		}
		
		if HaveEncounter {
			print "Encounter" at (0,26).
			break.
		}
	}

	set thrust to 0.
	print "BURN-I END!" at (0,25+5). wait 1.

	REMOVE nd.  
	//REMOVE ned2.
	
	LOG "PhaseI-Burn.c" to burn.txt.
}//DOTHEMAINBURN

if NOT EXISTS("burn.txt") or BODY:NAME = "Sun"
	DOTHEMAINBURN().

//LEM DOCKING -------------------------------------------------------
if vehicle_type = "SaturnV" and NOT EXISTS("dock.txt")
	RUNPATH( "boot/PhaseI-Docking.c" ).

//WARP TO "MOON" SOI -------------------------------------------------------

if vehicle_type = "Space4"
{
	SET MAPVIEW TO TRUE. wait 1.
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS OFF. wait 0.1.
	SET MAPVIEW TO FALSE. wait 2. set sasmode TO "PROGRADE". wait 1.
} else {
	AG5 ON. //Panels ON
}

CLEARSCREEN. print " ". print " ".
update_phase_title("Warp-Out "+BODY:NAME+" SOI", 1, true).
set x to 0.
set warp to 4. wait 0.1.
until x = 1 
{
	if body:name <> DEFAULT_KSC {
		{
			set warp to 0.
			set x to 1.
		}
	}

	print "body: " + body:name+"    " at (0,11).
	print "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"     " at (0,10). //FINAL
}

set warp to 0. wait 0.1.
print "PhaseI-Burn: ENDED!".