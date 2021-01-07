// --------------------------------------------------------------------------------------------
// Filename: PhaseII-Circularize.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Circularize an orbit after BURN.c
// 07/Jan/2021
// --------------------------------------------------------------------------------------------

parameter goto_mission_target.

//AG5 ON.
set TARGET to goto_mission_target. //BODY(goto_mission_target).
set warp to 0.
SAS OFF.

// Altitudes INFO: https://forum.kerbalspaceprogram.com/index.php?/topic/173446-lowest-highest-points-of-celestial-bodies/
if (Orbit:periapsis > MAX(40000, 1.5*TARGET:atm:height)) // 40,000m is a safe altitude (to do a periapsis wait) in all planets.
{
	clearscreen.
	set SOI to TARGET:SOIRADIUS. // 2429559.1.	(MUN-SOI)
	print "PhaseII-Warp to "+goto_mission_target+" Periapsis".
	set warp to 5.
	wait until eta:periapsis < 2000.
	set warp to 4.
	wait until eta:periapsis < 500.
	set warp to 3.
	wait until eta:periapsis < 50.
	set warp to 0.

	lock steering to retrograde.
	LOCK STEERING TO SHIP:RETROGRADE.

	RCS ON.
	wait 5.
	RCS OFF.

	clearscreen.
	print "PhaseII-Burn for "+goto_mission_target+" Capture/Orbit".
	//-------------------------------------------------------------------------------
	wait until eta:periapsis < 1.
	lock throttle to 0.5.
	print "eta:periapsis < 1".
	
	wait until apoapsis < .5*SOI.
	print "until apoapsis < .5*SOI".
	
	lock throttle to 0.
	set thrust to 0.
	lock throttle to thrust.

	// Calculate Delta-V to circularize orbit:
	// TARGET:
	set GM to goto_mission_target:mu. //GM = 6.5138398*(10^10).  (MUN)
	set Rp to periapsis + goto_mission_target:RADIUS.
	set vcir to (GM/Rp)^.5.
	set Ra to apoapsis + goto_mission_target:RADIUS.
	set a to (Ra+Rp)/2.
	set e to (Ra-Rp)/(Ra+Rp).
	set h to (GM*a*(1-(e^2)))^.5.
	set Vp to h/Rp.
	set ar to (Vp^2)/Rp.
	set g to GM/(Rp)^2.
	set W to mass*(g-ar).
	
	set warp to 0. wait 5.
	
	PRINT "Press [ENTER], to continue...".
	set ch to terminal:input:getchar().
	
	if vehicle_type = "SaturnV"
	{ 
		RCS ON.
		AG8 ON. WAIT 5.
		// STAGE. WAIT 5.
		// STAGE. WAIT 5.
		print "CYCLE UNTIL POSITIVE TRUST...".
		until maxthrust > 0 
			{ confirm_stage(). wait 5. }
	}

	set theta to arcsin(W/maxthrust).
	RCS OFF.
	
	// I do two seperate burns to help make the burns more efficient
	// It is more efficient to burn at Periapsis Warp!
	clearscreen.
	print "PhaseII-Circularize-Warp to Periapsis".
	print "theta: "+theta.
	
	set warp to 6.
	print "warp to 6".
	wait until eta:periapsis < 2000.

	set warp to 4.
	print "warp to 4".
	wait until eta:periapsis < 500.
	
	set warp to 3.
	print "warp to 3".
	wait until eta:periapsis < 50.
	
	set warp to 0.
	lock steering to heading (270, theta).
	
	// Waiting on periapsis arrival.
	print "Vertical Speed" at (0,10).
	until verticalspeed > 0 {
		print verticalspeed at (20,10).
	}.
	
	clearscreen.
	update_phase_title("PhaseII-Prepare", 0, false).
	SAS OFF. wait 0.1.
	lock steering to RETROGRADE. wait 0.1.
	wait 20.
	set thrust to 1.
	
	wait until status <> "ESCAPING".
	
	set ly to 3.
	
	// Burn to circularize, theta is used to maintain the apogee infront of the craft
	print "Burn to Circularize Orbit" at (0,ly+0).
	print "Vertical Speed" at (0,ly+1).
	print "Orbital Speed" at (0,ly+2).
	print "Vcir" at (0,ly+3).
	print ROUND(vcir) at (20,ly+3).
	print "Theta" at (0,ly+4).
	print ROUND(theta) at (20,ly+4).
	print "STATUS" at (0,ly+5).
	set y to .5.
	set Vo to 1000.
	set z to 0.
	set x to 1.
	set r to altitude+goto_mission_target:radius. //600000.
	until Vo-vcir < .001 
	{
		set thrust to x.
		set vorbit to velocity:orbit.
		set Vox to vorbit:x.
		set Voy to vorbit:y.
		set Voz to vorbit:z.
		set Vo to ((Vox^2)+(Voy^2)+(Voz^2))^.5.
		set ar to (Vo^2)/r.
		set W to mass*(g-ar).
		
		// Generic:
		if throttle > 0 AND maxthrust = 0 //AND vehicle_type < 3 
		{
			confirm_stage(). // Decouple
			wait 5.
		}
		if y = .5 {
			set err to .75.
			set error to 1-(err*verticalspeed).
			set theta to arcsin(W/maxthrust).
			set theta to theta*error.
			}.
		if (Vo-vcir) < 100  and y < 1{
			set err to 4.
			set A to 10.
			set y to y+1.
			}.
		if (Vo-vcir) < 10 and y < 2{
			set err to 5.
			set A to 1.
			set y to y+1.
			}.
		if (Vo-vcir) < 1 and y < 3{
			set err to 6.
			set A to .1.
			set y to y+1.
			}.
		if y > 1 and vehicle_type < 2
		{
			set error to 1-(err*verticalspeed).
			set C to mass*A.
			set B to ((W^2)+(C^2))^.5.
			set x to B/maxthrust.
			if x > 1 {
				set x to 1.
				}.
			set theta to arctan(W/C).
			set theta to theta*error.
		}

		print ROUND(verticalspeed,2) at (20,ly+1).
		print ROUND(Vo,2) at (20,ly+2).
		print ROUND(theta,2) at (20,ly+4).
		print STATUS at (20,ly+5).
		//wait 1.
	}
		
	lock throttle to 0.
	RCS OFF.

	// set e to (apoapsis-periapsis)/(apoapsis+periapsis).
	// print "Eccentricity" at (0,0). print e at (20,0).
	// set avg to (apoapsis+periapsis)/2-FINAL.
	// set error to avg/FINAL*100.
	// print "Error " + error + "%" at (0,1).
	print "Craft is now in stable circular orbit around "+goto_mission_target at (0,20).
}.

print "This ends Phase II, on to Phase III" at (0,21). 