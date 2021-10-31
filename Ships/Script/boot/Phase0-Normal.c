// --------------------------------------------------------------------------------------------
// Filename: Phase0-Normal.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is used to match the target orbit inclination.
// 31/Oct/2021
// --------------------------------------------------------------------------------------------

parameter mission_target_parameter.
set TARGET to mission_target_parameter. //set TARGET to BODY(mission_target_parameter).

RCS ON.
set dv_factor to 1.
set BURN to 0. //NODE(0, 0, 0, 0).

//----------------------------------------------------------------------------------
function getNormalOrbitAngle 
{
	set a1 to sin(ship:Orbit:Inclination)*cos(ship:orbit:LAN).
	set a2 to sin(ship:Orbit:Inclination)*sin(ship:orbit:LAN).
	set a3 to cos(ship:Orbit:Inclination).

	set b1 to sin(target:Orbit:Inclination)*cos(target:orbit:LAN).
	set b2 to sin(target:Orbit:Inclination)*sin(target:orbit:LAN).
	set b3 to cos(target:Orbit:Inclination).

	set angle to arccos(a1*b1+a2*b2+a3*b3).
	
	return angle.
}

//----------------------------------------------------------------------------------
function eta_true_anom 
{
	declare local parameter tgt_lng.
	// convert the positon from reference to deg from PE (which is the true anomaly)
	local ship_ref to mod(obt:lan+obt:argumentofperiapsis+obt:trueanomaly,360).
	// s_ref = lan + arg + referenc

	local node_true_anom to (mod (720+ tgt_lng - (obt:lan + obt:argumentofperiapsis),360)).

	local node_eta to 0.
	local ecc to OBT:ECCENTRICITY.
	print "Node anomaly   : " + round(node_true_anom,2).	
	print "ecc   : " + round(ecc,2).	
	if ecc < 0.001 {
		set node_eta to SHIP:OBT:PERIOD * ((mod(tgt_lng - ship_ref + 360,360))) / 360.
	} else {
		if ((ecc + cos(node_true_anom)) / (1 + ecc * cos(node_true_anom)) <= 1)
		{
			local eccentric_anomaly to	arccos((ecc + cos(node_true_anom)) / (1 + ecc * cos(node_true_anom))).
			local mean_anom to (eccentric_anomaly - ((180 / (constant():pi)) * (ecc * sin(eccentric_anomaly)))).
		
			// time from periapsis to point
			local time_2_anom to  SHIP:OBT:PERIOD * mean_anom /360.

			local my_time_in_orbit to ((OBT:MEANANOMALYATEPOCH)*OBT:PERIOD /360).
			set node_eta to mod(OBT:PERIOD + time_2_anom - my_time_in_orbit,OBT:PERIOD) .
		} else set node_eta to -1.
	}
	
	return node_eta.
}

//----------------------------------------------------------------------------------
function set_inc_lan 
{
	DECLARE PARAMETER incl_t.
	DECLARE PARAMETER lan_t.
	
	local node_eta to -1.
	local Va to 0.
	local Vb to 0.
	UNTIL node_eta > -1 {

		local incl_i to SHIP:OBT:INCLINATION.
		local lan_i to SHIP:OBT:LAN.

		// setup the vectors to highest latitude; Transform spherical to cubic coordinates.
		set Va to V(sin(incl_i)*cos(lan_i+90),sin(incl_i)*sin(lan_i+90),cos(incl_i)).
		set Vb to V(sin(incl_t)*cos(lan_t+90),sin(incl_t)*sin(lan_t+90),cos(incl_t)).
		// important to use the reverse order
		local Vc to VCRS(Vb,Va).

		set dv_factor to 1.
		//compute burn_point and set to the range of [0,360]
		local node_lng to mod(arctan2(Vc:Y,Vc:X)+360,360).
		local ship_ref to mod(obt:lan+obt:argumentofperiapsis+obt:trueanomaly,360).

		local ship_2_node to mod((720 + node_lng - ship_ref),360).
		if ship_2_node > 180 {
			print "Switching to DN".
			set dv_factor to -1.
			set node_lng to mod(node_lng + 180,360).
		}

		local node_true_anom to 360- mod(720 + (obt:lan + obt:argumentofperiapsis) - node_lng , 360 ).
		local ecc to OBT:ECCENTRICITY.
		local my_radius to OBT:SEMIMAJORAXIS * (( 1 - ecc^2)/ (1 + ecc*cos(node_true_anom)) ).
		local my_speed1 to sqrt(SHIP:BODY:MU * ((2/my_radius) - (1/OBT:SEMIMAJORAXIS)) ).	
		set node_eta to eta_true_anom(node_lng).
		if (node_eta = -1) {
				print "ERROR: node_eta = -1, trying again.".
				//set node_eta to 10.
				//return.
				wait 15.
		}
	}
	
	local my_speed to VELOCITYAT(SHIP, time+node_eta):ORBIT:MAG.
	local d_inc to arccos (vdot(Vb,Va) ).
	local dvtgt to dv_factor* (2 * (my_speed) * SIN(d_inc/2)).

	// Create a blank node
	local inc_node to NODE(node_eta, 0, 0, 0).
	//	we need to split our dV to normal and prograde
 	set inc_node:NORMAL to dvtgt * cos(d_inc/2).
	// always burn retrograde
	set inc_node:PROGRADE to 0 - abs(dvtgt * sin(d_inc/2)).
	set inc_node:ETA to node_eta.

	ADD inc_node.
	set BURN to inc_node.
}

//----------------------------------------------------------------------------------
function Inclination_Match 
{
	//RESET: thrust
	set thrust to 0.	
	lock throttle to thrust.
	wait 1.
	
	set_inc_lan (target:orbit:inclination, target:orbit:lan). //GET: BURN NODE
	print "BURN:ETA "+ ROUND(BURN:ETA,1).
	
	warp_until_node (BURN).
	
	set warp to 0. wait 1.
	SET MAPVIEW TO FALSE. wait 1.  // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	RCS ON. wait 0.1.
	
	set prev_angle to getNormalOrbitAngle().
	
	// set sasmode to "maneuver".
	wait 0.
	if dv_factor < 0
	{
		set sasmode to "ANTINORMAL". wait 0.1.
		print "SAS ANTINORMAL".
	} else {
		set sasmode to "NORMAL". wait 0.1.//set sasmode to "maneuver".
		print "SAS NORMAL".
	}
	
	if vehicle_type = "Space4"
		WAIT 30.
	else
		WAIT 15.
	
	set thrust to 1.
	SET PREV_TIME to TIME:SECONDS.
	print "PREV_TIME: "+PREV_TIME  + "        "  at (0,10).

	until TIME:SECONDS > PREV_TIME + 35
	{
		set angle to getNormalOrbitAngle().

		if angle > -0.5 and angle < 0.5 or angle*angle > (prev_angle+1)*(prev_angle+1)
		{
			set thrust to 0.
			remove BURN.
			wait 1.
			return 1.
		}.

		print "Normal Orbit (angle): "+ ROUND(angle,1)  + "        "  at (0,11).
		print "(dv_factor):"+dv_factor+ "        "  at (0,12).
	}.

	set thrust to 0.
	remove BURN.
	wait 1.
	return 0.
}.

////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN:
////////////////////////////////////////////////////////////////////////////////////////////////
clearscreen.
print " ".print " ".print " ".
update_phase_title("Inclination Burn", 0, true).

if vehicle_type = "Space4"
{ 
	UNLOCK STEERING. WAIT 0.1. 
	SAS ON. WAIT 0.1. 
}

set result to Inclination_Match(). print result at (0,14).
print "SUMMARY: TARGET Inclination Corrected: (result) "+result.

//RESET:
UNTIL NOT HASNODE { REMOVE NEXTNODE. WAIT 0. } //removeAllNodes!
SAS OFF.
RCS OFF.
set warp to 0.
