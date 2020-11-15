// --------------------------------------------------------------------------------------------
// Filename: Falcon-Return.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              	- Land the Falcon(s) ST-1
// 15/Nov/2020
// --------------------------------------------------------------------------------------------
// Tested: 
// 13/Nov/2020 with ->0v2.sfs
// 11/Nov/2020 with ->2_1v2.sfs

function boostback_burn
{
	parameter do_reverse to false.
	
	update_phase_title("BOOSTBACK BURN "+STAGE_1_TYPE, 1).
	set warp to 0.
	SAS OFF.
	set impactDist to 999999.
	
	until FALSE
	{
		SET prev_impactDist to impactDist.
		
		if STAGE_1_TYPE = "CORE" {
			//270: LandingTarget:HEADING+180
			LOCK STEERING TO HEADING(270,0, -270). //DUE LandingTarget:BEARING calc bug, dont used it!
			if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
				SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS:TR:IMPACTPOS).
		} else
			steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset, do_reverse). // Calculate: impactDist
		
		if(impactDist < 3000)
		{
			PRINT "OP3: impactDist < 3km   " at (0,2).
			SET thrust TO 0.05.	// Near? for high precision, do it in lower thrust
			RCS ON.
		}else if(impactDist < 10000){
			PRINT "OP2: impactDist < 10km  " at (0,2).
			SET thrust TO 0.1.
		}else{
			PRINT "OP1: impactDist > 10km   " at (0,2).
			SET thrust TO 1.	// Faraway? all seconds count do it ASAP
		}
		
		if (do_reverse)
		{
			if SHIP:GROUNDSPEED < 200
				break.
		}else{
			if (impactDist < 1000) and (impactDist > prev_impactDist)
				break.
		}
		
		PRINT_STATUS (3).
	}
	
	SET thrust TO 0.
}

////////////////////////////////////////////////////////////////////////////////////////////////
function ReEntryburn 
{
	parameter safe_alt.
	parameter safe_power to 1.
	parameter maxDescendSpeed to 100.	//Due Terminal Speed don't use another above value.
	
	update_phase_title("WAIT4RE-ENTRY BURN", 1).
	set x to 0.
	
	sas off.
	until SHIP:ALTITUDE < 3000
	{
		SET prev_impactDist to impactDist.
		updateHoverSteering().	
		
		if SHIP:ALTITUDE < safe_alt or SHIP:VERTICALSPEED < -1800
		{
			//REENTRY BURN!!!
			if  x = 0 
			{
				update_phase_title("REENTRY BURN", 1).
				set warp to 0.
				set x to 1.
				RCS ON.
				SAS OFF.
				
				SET thrust to safe_power .
			}
			
			if impactDist > 250
			{
				steerToTarget(steeringPitch, 0, 0). // Calculate: impactDist
			} else {
				steerToTarget(82.5, 0, 0). 			// Calculate: impactDist
			}
		} else {
			lock steering to retrograde.
				
			if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
				SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS:TR:IMPACTPOS).
		}
		
		if SHIP:VERTICALSPEED >-200 and (impactDist > prev_impactDist)
		{
			SET thrust to 0.
			LOCK STEERING TO HEADING(270,0, -270).
			SET thrust to 0.01.
			activateOneEngine().			
		
			update_phase_title("FINE TUNE BURN", 0).
			SAS OFF.
			until (impactDist < 25) or (SHIP:VERTICALSPEED >= 0)
			{
				SET prev_impactDist to impactDist.
				
				if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
					SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS:TR:IMPACTPOS).
				
				updateHoverSteering().
				steerToTarget(85).
				set error to 0.825.
				if maxthrust = 0
					set t to 1.
				else
					set t to error*((1000*SHIP:MASS*g)/maxthrust)/maxthrust.
				
				setHoverDescendSpeed(1+((alt:radar-29)/10),t).
				
				PRINT_STATUS (3, t).
				
				if (impactDist > prev_impactDist) //Results worst than before? exit...
					 break.
			}
			
			SET thrust to 0.
			break.
		}

		PRINT_STATUS (3).
	}
	
	UNLOCK STEERING.
}


function waitAndDoReEntryburn 
{
	update_phase_title("STEERING PROGRADE", 0).
	if SHIP:altitude > 100000
	{
		until (SHIP:VERTICALSPEED < 0) 	//(SHIP:altitude > 140000) or 
		{
			if STAGE_1_TYPE <> "SLAVE" 
			{
				RCS ON. wait 0.5.
				SAS ON. wait 0.5.
				set sasmode TO "PROGRADE". wait 0.5.
			} else {
				SAS OFF.
				LOCK STEERING TO UP + R(0,0,180).
			}
		
			if SHIP:altitude > BODY:ATM:HEIGHT 
			{
				RCS OFF.
				wait 1.
				set warp to 2.
			}

			// Wait until out ATM...
			until (SHIP:altitude > 140000) or (SHIP:VERTICALSPEED < 0)
				PRINT_STATUS (3).

			// Warp until re-enter in atmosphere:
			until (warp = 0) WAIT 1.  // Wait until return again to ATM...
		}
	}



	update_phase_title("FOCUS ON TARGET", 0).
	UNLOCK STEERING.
	WAIT 2.
	SAS OFF.
	WAIT 2.
	if STAGE_1_TYPE <> "SLAVE" 
	{
		UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
		SAS ON.
		WAIT 2.
		SET TARGET TO LandingZone.
		WAIT 2.
	}
	else
		set LandingTarget TO offline_LandingZone1.

	RCS ON.
	update_phase_title("(GRID FINS DEPLOY)", 0).
	BRAKES ON.

	// Major Correction:
	PRINT_STATUS(3).

	set burnAlt to 32000.
	if (SHIP:altitude > burnAlt)
		ReEntryburn(burnAlt).
}

////////////////////////////////////////////////////////////////////////////////////////////////
function aerodynamic_guidance 
{
	update_phase_title("AERODYNAMIC GUIDANCE", 1).
	RCS ON.	wait 1.
	until (SHIP:GROUNDSPEED < 1) or(SHIP:ALTITUDE <= (sBurnDist*1.3)) or impactTime <= 13
	{
		if (impactDist < 50)
		{
			LOCK STEERING TO UP + R(0,0,180).
		} else {
			SAS OFF.
			updateHoverSteering().
			steerToTarget(steeringPitch/1.1, 0, 0, true). // Calculate: impactDist
		}
			
		if ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
			SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS:TR:IMPACTPOS).
		else
			SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), COM_ADDONS_TR_IMPACTPOS).
		
		if altitude <= 10000 and (SHIP:GROUNDSPEED > 50) and impactDist > 500 
			break. // Just Aerodynamic Guidance not enouf...
		
		PRINT_STATUS (3).
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
function landingBurn
{
	parameter landing_burn to 1000.
	parameter maxDescendSpeed TO 125.
	
	update_phase_title("LANDING BURN", 1).
	SAS OFF.
	RCS ON.
	until (alt:radar<100 and Verticalspeed=0)
	{
		if impactDist > 100
		{
			setHoverMaxSteerAngle(10).
			setHoverMaxHorizSpeed(10).
		} else {
			setHoverMaxSteerAngle(5).
			setHoverMaxHorizSpeed(5).
		}
		
		updateHoverSteering().
		if impactDist > 500 
		{
			steerToTarget(steeringPitch).
			set maxDescendSpeed to 25.
		} else {
			steerToTarget(80).
			set maxDescendSpeed to 125.
		}
		
		set error to 0.825. //Keep up @82.5% x g
		if maxthrust = 0
			set t to 1.
		else
			set t to error*((1000*SHIP:MASS*g)/maxthrust)/maxthrust.
		if (alt:radar>landing_burn)
			setHoverDescendSpeed(maxDescendSpeed, t).
		else
			setHoverDescendSpeed(15, t).
		 
		PRINT_STATUS (3, t). 
		
		if ((impactDist < 150) and (alt:radar<(landing_burn*0.9))) or (alt:radar<(landing_burn*0.9))
			break.
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
function touchdown 
{
	SAS OFF.
	update_phase_title("TOUCHDOWN",1).
	SET step to TRUE.
	set checkgear to 0.
	until (SHIP:STATUS="LANDED") or sBurnDist <= 0.1
	{
		if alt:radar > 650 and (impactDist < 150) and SHIP:GROUNDSPEED < 5 and Verticalspeed > -25
			set thrust to 0. // we are too high! we need to gain vertical speed a bit 
		
		if alt:radar < 250 and checkgear = 0 {
			GEAR ON.
			set checkgear to 1.
		}
			
		if (alt:radar-30) < 250 and LandingTarget:DISTANCE > 100
			set rate to steeringPitch.	//FAST correction
		else
			set rate to 80.				//MEDIUM correction

		if GROUNDSPEED < 5 and impactDist < 50
			set rate to 85.				//SLOW correction

		updateHoverSteering().
		steerToTarget(rate).

		//Calc Optimized Throttle:
		if (alt:radar<=60) 
			set error to 0.5.
		else
			set error to 0.825.
		if maxthrust = 0
			set t to 1.
		else
			set t to error*((1000*SHIP:MASS*g)/maxthrust)/maxthrust.
		
		setHoverDescendSpeed(2+((alt:radar-30)/7.5),t).
		
		PRINT_STATUS (3, t). 
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
function rocketshutdown
{
	SAS OFF.
	update_phase_title("SECURE ROCKET",1).
	SET thrust to 0.
	unlock throttle.							//Auto Throttle
	SET SHIP:CONTROL:PILOTMAINTHROTTLE TO 0.	//Pilot Throttle Lever
	UNLOCK STEERING.
	shutDownAllEngines().
	if KUniverse:ActiveVessel = SHIP
		set target to BODY("Earth").
	SAS ON.
	WAIT 3.
	SAS OFF.
	RCS OFF.
}

////////////////////////////////////////////////////////////////////////////////////////////////
// START:
////////////////////////////////////////////////////////////////////////////////////////////////
CLEARSCREEN.
update_phase_title("(Return Home)  ",0).
SET thrust TO 0.
lock throttle to thrust.

steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset). // Calculate: impactDist
if (impactDist > 5000) and (SHIP:altitude > 50000) 
{
	if STAGE_1_TYPE = "CORE"
		boostback_burn(true).
	else
		boostback_burn().
}

AG8 ON. //Disable Lower RCS.
waitAndDoReEntryburn().
activateOneEngine().
aerodynamic_guidance().
landingBurn(). //3000
touchdown().
rocketshutdown().