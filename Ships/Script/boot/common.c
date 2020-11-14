// --------------------------------------------------------------------------------------------
// Filename: common.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Adapted code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Original code:   - https://drive.google.com/drive/folders/0BzhWP-nZiAuvZHNmWnBSQ0dVSVE
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              	- Common lib of functions used by Falcon-Return.c
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

declare global landingAltitude TO LandingTarget:TERRAINHEIGHT.
declare global coreAdjustLatOffset TO 0.
declare global coreAdjustLngOffset TO 0.
declare global impactDist TO LandingTarget:DISTANCE.
declare global impactTime TO 0.
declare global overshootLatLng TO LATLNG(0, 0).
declare global coreSburnStart TO 1900.
declare global sBurnDist TO 0.
declare global burnAlt TO 0.
declare global radarOffset to 29.//the radar altitude Kerbal Engineer (or kOS itself gives) when landed with he legs extended.				

set steeringDir to 0.
SET steeringPitch to 0.
SET shipPitch TO 0.	

function setHoverPIDLOOPS
{
	// KP=The proportional gain factor
	// KI=The integral gain factor
	// KD=The derivative gain factor
	
	// kp = 1, ki = 0, kd = 0
	// maxoutput = maximum number value
	// minoutput = minimum number value
	
	// SET PID TO PIDLOOP(KP).
	// SET PID TO PIDLOOP(KP, KI, KD).
	// SET PID TO PIDLOOP(KP, KI, KD, MINOUTPUT, MAXOUTPUT).													  
	
	SET climbPID TO PIDLOOP(0.1, 0.3, 0.005, 0, 1). 	//Controls vertical speed
	SET hoverPID TO PIDLOOP(1, 0.01, 0.0, -50, 50). 	//Controls altitude by changing climbPID setpoint
	SET hoverPID:SETPOINT TO landingAltitude. 			//is the altitude of landing pad
	
	SET eastVelPID TO PIDLOOP(3, 0.01, 0.0, -35, 35).	//Controls horizontal speed by tilting rocket
	SET northVelPID TO PIDLOOP(3, 0.01, 0.0, -35, 35). 
	
	SET eastPosPID TO PIDLOOP(1700, 0, 100, -40,40).	//controls horizontal position by changing velPID setpoints
	SET northPosPID TO PIDLOOP(1700, 0, 100, -40,40).
	
	SET eastPosPID:SETPOINT TO LandingTarget:LNG.
	SET northPosPID:SETPOINT TO LandingTarget:LAT.
}

function horizontalDirection
{
	parameter geo1. parameter geo2.
	return ARCTAN2(geo1:LNG - geo2:LNG, geo1:LAT - geo2:LAT).
}
function horizontalDistance 
{
	parameter geo1. parameter geo2.
	return (geo1:POSITION - geo2:POSITION):MAG.
}
function steerToTarget
{
	parameter pitch is 1. parameter overshootLatModifier is 0 .parameter overshootLngModifier is 0. parameter do_reverse to false.
	
	if STAGE_1_TYPE = "CORE"
		set limit to 57.
	else
		set limit to 30.
	
	if alt:radar <= limit
	{
		LOCK STEERING TO UP + R(0,0,180).
		return.
	}
	
	SET overshootLatLng TO LATLNG(LandingTarget:LAT + overshootLatModifier, LandingTarget:LNG + overshootLngModifier).
	
	if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
	{
		SET impactDist TO horizontalDistance(overshootLatLng, ADDONS:TR:IMPACTPOS).
		SET targetDir TO horizontalDirection(ADDONS:TR:IMPACTPOS,overshootLatLng).
	} else{
		SET impactDist TO horizontalDistance(overshootLatLng, COM_ADDONS_TR_IMPACTPOS).
		SET targetDir TO horizontalDirection(COM_ADDONS_TR_IMPACTPOS,overshootLatLng).
	}
	
	SET steeringDir TO targetDir - 180.
	if (do_reverse)
		LOCK STEERING TO HEADING(steeringDir+180,pitch, -270).
	else
		LOCK STEERING TO HEADING(steeringDir,pitch, -270).
}

function setHoverMaxSteerAngle
{
	parameter a.
	
	SET eastVelPID:MAXOUTPUT TO a.
	SET eastVelPID:MINOUTPUT TO -1*a.
	SET northVelPID:MAXOUTPUT TO a.
	SET northVelPID:MINOUTPUT TO -1*a.
}
function setHoverMaxHorizSpeed
{
	parameter a.
	SET eastPosPID:MAXOUTPUT TO a.
	SET eastPosPID:MINOUTPUT TO -1*a.
	SET northPosPID:MAXOUTPUT TO -1*a. 	
	SET northPosPID:MINOUTPUT TO a.		
}

function sProj
{ 
	parameter a. parameter b.
	
	if b:mag = 0 
		RETURN 1.
	else
		RETURN VDOT(a, b) * (1/b:MAG).
}
function cVel 
{
	local v IS SHIP:VELOCITY:SURFACE.
	local eVect is VCRS(UP:VECTOR, NORTH:VECTOR).
	local eComp IS sProj(v, eVect).
	local nComp IS sProj(v, NORTH:VECTOR).
	local uComp IS sProj(v, UP:VECTOR).
	RETURN V(eComp, uComp, nComp).
}

function updateHoverSteering
{
	SET cVelLast TO cVel().
	SET eastVelPID:SETPOINT TO eastPosPID:UPDATE(TIME:SECONDS, SHIP:GEOPOSITION:LNG).
	SET northVelPID:SETPOINT TO northPosPID:UPDATE(TIME:SECONDS,SHIP:GEOPOSITION:LAT).
	
	LOCAL eastVelPIDOut IS eastVelPID:UPDATE(TIME:SECONDS, cVelLast:X).
	LOCAL northVelPIDOut IS northVelPID:UPDATE(TIME:SECONDS, cVelLast:Z).
	
	LOCAL eastPlusNorth is MAX(ABS(eastVelPIDOut), ABS(northVelPIDOut)).
	if STAGE_1_TYPE = "SLAVE" 
		SET steeringPitch TO COM_steeringDir.
	else {
		SET steeringPitch TO 90 - eastPlusNorth.
	}
	
	LOCAL steeringDirNonNorm IS ARCTAN2(eastVelPID:OUTPUT, northVelPID:OUTPUT).
	if steeringDirNonNorm >= 0
		SET steeringDir TO steeringDirNonNorm.
	else
		SET steeringDir TO 360 + steeringDirNonNorm.
}

function setHoverDescendSpeed
{
	parameter a. parameter minThrott is 0.
	
	SET hoverPID:MAXOUTPUT TO a.
	SET hoverPID:MINOUTPUT TO -1*a.
	SET climbPID:SETPOINT TO hoverPID:UPDATE(TIME:SECONDS, SHIP:ALTITUDE). //control descent speed with throttle
	
	SET calcThrott TO climbPID:UPDATE(TIME:SECONDS, SHIP:VERTICALSPEED).
	if(calcThrott<minThrott)
		SET calcThrott TO minThrott.
	
	if impactDist >150 and GROUNDSPEED >20 and alt:radar < 500
		set calcThrott to calcThrott*0.9.
		
	SET thrust TO calcThrott.	
}

function updateLandingVars  //Scalar projection of two vectors. Find component of a along b. a(dot)b/||b||
{
	updateVars().
	SET HorizDist TO horizontalDistance(landingTarget, SHIP:GEOPOSITION).
	            // Get angle between:  UP      SHIP:Delta
	SET shipPitch TO 90 - vang(SHIP:up:vector, SHIP:facing:forevector).
	SET distMargin TO 0.
	SET maxVertAcc TO (SHIP:AVAILABLETHRUST) / SHIP:MASS - g. //max acceleration in up direction the engines can create
	SET vertAcc TO sProj(SHIP:SENSORS:ACC, UP:VECTOR).
	SET dragAcc TO g + vertAcc. //vertical acceleration due to drag. Same as g at terminal velocity
	SET sBurnDist TO (SHIP:VERTICALSPEED^2 / (2 * (maxVertAcc + dragAcc/2)))+distMargin.
	if sBurnDist < 0 
		set sBurnDist to 0.
}

function PRINT_STATUS 
{
	parameter y.
	parameter t to 0.
	
	updateLandingVars().
	
	if STAGE_1_TYPE <> "SLAVE"
		PRINT "TARGET: "+LandingZone:NAME at (0,y).
	else
		PRINT "TARGET: "+LandingZone_NAME at (0,y).
	
	PRINT "LandingTarget:LAT: "+ROUND (LandingTarget:LAT,1)+" deg.   " at (0,y+1).
	PRINT "LandingTarget:LNG: "+ROUND (LandingTarget:LNG,1)+" deg.   " at (0,y+2).
	PRINT "LandingTarget:DISTANCE: "+ROUND(LandingTarget:DISTANCE/1000,3)+" km   " at (0,y+3).
	PRINT "LandingTarget:TERRAINHEIGHT: "+ROUND (landingAltitude,1)+" m   " at (0,y+4).
	PRINT "LandingTarget:HEADING: "+ROUND (LandingTarget:HEADING,1)+" deg.   " at (0,y+5).
	PRINT "LandingTarget:BEARING: "+ROUND (LandingTarget:BEARING,1)+" deg.   " at (0,y+6).
	
	PRINT "Landing distance from target: " + ROUND(impactDist/1000,3) +" km   " at (0,y+8).
	PRINT "Current distance from target: "+ROUND(HorizDist/1000,3)+" km   " at (0,y+9).
	PRINT "landingAltitude: "+ROUND (landingAltitude,1)+" m   " at (0,y+10).
	PRINT "alt:radar: "+ROUND (alt:radar,1)+" m   " at (0,y+11).
	PRINT "steeringDir: "+ROUND (steeringDir,1)+"   " at (0,y+12).
	PRINT "steeringPitch: "+ROUND (steeringPitch,1)+"   " at (0,y+14).
	PRINT "shipPitch: "+ROUND (shipPitch,1)+"   " at (0,y+15).
	
	PRINT "throttle(maxthrust): " + ROUND(throttle,2) + " ("+ROUND(maxthrust)+")     " at (0,16).
	
	PRINT "sBurnDist: "+ROUND (sBurnDist,1)+" m   " at (0,y+17).
	PRINT "Groundspeed: "+ROUND (GROUNDSPEED,1)+" m/s   " at (0,y+18).
	PRINT "Altitude: "+ROUND(SHIP:ALTITUDE)+"  " at (0, y+19).
	PRINT "Verticalspeed: "+ROUND(SHIP:VERTICALSPEED)+"   " at (0, y+20).
	
	set trueRadar to alt:radar - radarOffset.//this is all the suicide burn calculation					
	set maxDecel to (ship:availablethrust / ship:mass) - g.	
	set stopDist to ship:verticalspeed^2 / (2 * maxDecel).		
	set idealThrottle to stopDist / trueRadar.					
	set impactTime to trueRadar / abs(ship:verticalspeed).	
	PRINT "impactTime: "+ROUND(impactTime,1)+"   " at (0, y+21).
	PRINT "burnAlt: "+ROUND(burnAlt)+"   " at (0, y+22).
	PRINT "[t]: "+ROUND(t,2)+"   " at (0, y+23).
	
	if STAGE_1_TYPE = "MASTER"
	{
		if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT //and STAGE_1_TYPE <> "SLAVE"
			SLAVE_CONNECTION:SENDMESSAGE(list(throttle,steeringDir,shipPitch, ADDONS:TR:IMPACTPOS, SHIP:altitude)).
		
	} 
}

setHoverPIDLOOPS().