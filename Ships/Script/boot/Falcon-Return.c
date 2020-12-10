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
// 09/Dez/2020
// --------------------------------------------------------------------------------------------

// REGRESSION TESTS for KOS, Automatic Pilot Orbit and Landing:
// ------------------------------------------------------------
// [ok] F9 QMAX							1.20.12.05
// [ok] F9 ST-1 STAGE					1.20.12.05
// [ok] F9 ST-1 LANDING					1.20.12.05
// [ok] F9 ST-2 LEO ORBIT				1.20.11.21

// [ok] Crew Dragon 2 QMAX				1.20.12.05
// [ok] Crew Dragon 2 STAGE				1.20.12.05
// [ok] Crew Dragon 2 ST-1 LANDING		1.20.12.05
// [ok] Crew Dragon 2 DRAGON LEO ORBIT	1.20.11.21

// [ok] FH ST1 QMAX						1.20.12.07
// [ok] FH ST1 STAGE					1.20.12.07
// [ok] FH ST1 Master ST-1 LANDING		1.20.11.23
// [ok] FH Core ST-1 LANDING			1.20.12.07
// [ok] FH LEO Orbit					1.20.11.22
// [ok] FH GSO Orbit					1.20.12.07

// [not working] Slave ST-1 LANDING		Checking the impossible (at KSP) land of both ST-1 at same time...

////////////////////////////////////////////////////////////////////////////////////////////////
// INIT:
////////////////////////////////////////////////////////////////////////////////////////////////
SET thrust TO 0.
lock throttle to thrust.

set TOTAL_PARTS to 0.
FOR P IN SHIP:PARTS
	SET TOTAL_PARTS to TOTAL_PARTS + 1.

////////////////////////////////////////////////////////////////////////////////////////////////
function boostback_burn
{
	parameter do_reverse to false.
	parameter do_reverse_max_speed to 100.
	
	if do_reverse
		update_phase_title("BOOSTBACK BURN REV."+do_reverse_max_speed, 1).
	else
		update_phase_title("BOOSTBACK BURN NOR."+do_reverse_max_speed, 1).
	
	set warp to 0.
	SAS OFF.
	
	set impactDist to 999999.
	LOG  STAGE_1_TYPE + " - boostback_burn("+do_reverse_max_speed+") - START" to LOG.txt.
	
	set we_are_done to FALSE.
	until we_are_done
	{
		PRINT_STATUS (3).
		
		SET prev_impactDist to impactDist.

		if STAGE_1_TYPE = "CORE"
		{
			set lat_correction to 0.
			// if KUniverse:ActiveVessel = SHIP and ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
				// set ADDONS_TR_IMPACTPOS to COM_ADDONS_TR_IMPACTPOS.
				
			set lat_correction to (LandingTarget:LAT - ADDONS_TR_IMPACTPOS:LAT)*50.
			
			LOCK STEERING TO HEADING(270+lat_correction,0, -270).
			//if KUniverse:ActiveVessel = SHIP and ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
			SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
		} else
			steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset, do_reverse). // Calculate: impactDist

		if(impactDist < 5000)
		{
			PRINT "OP3: impactDist < 3km   " at (0,2).
			SET thrust TO 0.05.	// Near? for high precision, do it in lower thrust
			RCS ON.
			if STAGE_1_TYPE = "SLAVE" and impactDist < 2500
			{
				SET thrust TO 0.
				LOG  "SLAVE:[we_are_done] " to LOG.txt.
				set we_are_done to TRUE.
			}
		}else if(impactDist < 10000){
			PRINT "OP2: impactDist < 10km  " at (0,2).
			SET thrust TO 0.1.
		}else{
			PRINT "OP1: impactDist > 10km   " at (0,2).
			SET thrust TO 1.	// Faraway? all seconds count do it ASAP
		}
		
		if (do_reverse)
		{
			if SHIP:GROUNDSPEED < do_reverse_max_speed or (SHIP:GROUNDSPEED < 350 and impactDist > prev_impactDist)
				break.
		}else{
			if (impactDist < 1000) and (impactDist > prev_impactDist)
				break.
		}
	}
	
	PRINT_STATUS (3). // Send MSG: STOP to "SLAVE" (if MASTER)
	SET SHIP:CONTROL:PILOTMAINTHROTTLE TO 0.	//Pilot Throttle Lever
	SET thrust TO 0.
	
	if STAGE_1_TYPE = "SLAVE" and SLAVE_STAGE = 0
	{
		LOG  "SET SLAVE_STAGE to 1." to SLAVE.TXT.
		LOG  "REBOOT:1" to LOG.TXT.
		reboot.
	}

	update_phase_title("BOOSTBACK BURN END", 1).
	LOG  STAGE_1_TYPE + " " + we_are_done + " - boostback_burn() - END" to LOG.txt.
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
		PRINT_STATUS (3).
		SET prev_impactDist to impactDist.
		updateHoverSteering().	
		
		if SHIP:ALTITUDE < safe_alt //or SHIP:VERTICALSPEED < -1800
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
				steerToTarget(steeringPitch, 0, 0). // Fast Correction
			} else {
				steerToTarget(82.5, 0, 0). 			// Slower Correction
			}
		} else {
			lock steering to retrograde.
				
			//if KUniverse:ActiveVessel = SHIP and ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
				SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
		}
		
		if SHIP:VERTICALSPEED >-200 and (impactDist > prev_impactDist)
		{
			SET thrust to 0.
			LOCK STEERING TO HEADING(270,0, -270).
			SET thrust to 0.01.
			activateOneEngine().			
		
			AG8 ON. //Enable Lower RCS.
			// FOR FH:
			//if shipPitch < 60
			{
				//Add 10 secs of vertical stability after REENTRY BURN
				update_phase_title("(vertical stability)", 0).
				SET thrust to 0.
				RCS ON.
				FROM {local x is 5.} UNTIL x = 0 STEP {set x to x-1.} DO 
				{
					//lock steering to retrograde.
					LOCK STEERING TO up + R(0,0,180). //UP
					wait 1.
					//if KUniverse:ActiveVessel = SHIP and ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
						SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
					PRINT_STATUS (3).
				}
			}
			AG8 ON. //Disable Lower RCS.
			
			break.
		}
	}
	
	SET thrust to 0.
}

////////////////////////////////////////////////////////////////////////////////////////////////
function waitAndDoReEntryburn 
{
	update_phase_title("STEERING PROGRADE", 0).
	
	if altitude > 100000 and VERTICALSPEED > 0
	{
		SAS ON. wait 1.
		SAS OFF.
		RCS ON.
		LOCK STEERING TO UP + R(0,0,180).
			
		// Wait until out ATM...
		until (altitude > BODY:ATM:HEIGHT) or (VERTICALSPEED < 0) //BODY:ATM:HEIGHT=140000m (EARTH)
		{
			WAIT 0.1.
			PRINT_STATUS (3).
		}
		
		if SHIP:altitude > BODY:ATM:HEIGHT 
		{
			RCS OFF.
			wait 1.
		}
				
		// if STAGE_1_TYPE = "CORE" or STAGE_1_TYPE = "ST-1"
			// set warp to 2.
		// else
		{
			set warp to 0.
			wait 1.
			
			if STAGE_1_TYPE = "MASTER"
				SET KUniverse:ACTIVEVESSEL TO SLAVE_BOOSTER.
		}
		
		// Warp until re-enter in atmosphere:
		update_phase_title("W8 4 RE-ENTER", 0).
		until (SHIP:altitude < BODY:ATM:HEIGHT )  //warp = 0
		{
			WAIT 0.1.  // Wait until return again to ATM...
			PRINT_STATUS(3).
		}
	}

	update_phase_title("FOCUS ON TARGET", 0).
	UNLOCK STEERING.
	WAIT 2.
	SAS OFF.
	WAIT 2.

	if STAGE_1_TYPE = "SLAVE"
		set LandingTarget TO offline_LandingZone1.

	RCS ON.
	update_phase_title("(GRID FINS DEPLOY)", 0).
	BRAKES ON.

	// Major Correction:
	PRINT_STATUS(3).

	if mass*1000 > 50000
		set burnAlt to 35000.
	else
		set burnAlt to 32000.
	if (SHIP:altitude > burnAlt)
		ReEntryburn(burnAlt).
}

////////////////////////////////////////////////////////////////////////////////////////////////
function aerodynamic_guidance 
{
	update_phase_title("AERODYNAMIC GUIDANCE", 1).
	if KUniverse:ActiveVessel = SHIP 
		SET NAVMODE TO "SURFACE".

	RCS ON.	wait 1.
	until (SHIP:GROUNDSPEED < 1) or(SHIP:ALTITUDE <= (sBurnDist*1.4)) or impactTime <= 15
	{
		PRINT_STATUS (3).
		
		if SHIP:ALTITUDE > 7000
			set aerodynamic_target to 100.
		else if SHIP:ALTITUDE > 3000 
			set aerodynamic_target to 50.
	
		if (impactDist < aerodynamic_target) and SHIP:ALTITUDE > 3500
		{
			LOCK STEERING TO UP + R(0,0,180).
		} else 
		{
			SAS OFF.
			updateHoverSteering().
			steerToTarget(steeringPitch, 0, 0, true). // Calculate: impactDist
		}
			
		// if KUniverse:ActiveVessel = SHIP and ADDONS:TR:AVAILABLE and ADDONS:TR:HASIMPACT
			SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
		// else
			// SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), COM_ADDONS_TR_IMPACTPOS).
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
		// set error for min. thrust
		if impactDist > 50
		{
			steerToTarget(steeringPitch).	//FAST correction
			set maxDescendSpeed to 25.
			set error to 0.825. 			//Keep up @82.5% x g
		} else {
			steerToTarget(80).				//MEDIUM correction
			set maxDescendSpeed to 125.
			set error to 0.65. 				//Keep up @65% x g
		}
		
		//need: error
		if maxthrust = 0
			set t to 1.
		else
			set t to error*((1000*SHIP:MASS*g)/maxthrust)/maxthrust.

		PRINT_STATUS (3, t). //need: t
		
		if impactDist > 50
		{
			setHoverMaxSteerAngle(10).
			setHoverMaxHorizSpeed(10).
		} else {
			setHoverMaxSteerAngle(5).
			setHoverMaxHorizSpeed(5).
		}
		
		updateHoverSteering().
	
		if (alt:radar>landing_burn)
			setHoverDescendSpeed(maxDescendSpeed, t).
		else
			break.
		 
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
	until (SHIP:STATUS="LANDED" or sBurnDist <= 0.1) and alt:radar < 100
	{
		PRINT_STATUS (3, t). 
		
		if alt:radar > 650 and (impactDist < 150) and SHIP:GROUNDSPEED < 5 and Verticalspeed > -25
		{
			set thrust to 0.01. // we are too high! we need to gain vertical speed a bit 
			wait until Verticalspeed < -15.
		}
		
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
		// if (alt:radar<=60) 
			// set error to 0.5.
		// else
			set error to 0.75. //Keep up @75% x g
		if maxthrust = 0
			set t to 1.
		else
			set t to error*((1000*SHIP:MASS*g)/maxthrust)/maxthrust.
		
		setHoverDescendSpeed(2+((alt:radar-30)/7.5),t).
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
	// Final Stability:
	SAS ON.
	WAIT 5.
	SAS OFF.
	RCS OFF.
}

////////////////////////////////////////////////////////////////////////////////////////////////
function after_landing
{
	CLEARSCREEN.
	set AFTER_LAND_TOTAL_PARTS to 0.
	FOR P IN SHIP:PARTS 
		SET AFTER_LAND_TOTAL_PARTS to AFTER_LAND_TOTAL_PARTS + 1.

	if TOTAL_PARTS = AFTER_LAND_TOTAL_PARTS and STATUS <> "SPLASHED"
		PRINT "GRATZ: Another Booster landed safely!".
	else
		PRINT "INTEGRITY: "+(AFTER_LAND_TOTAL_PARTS/TOTAL_PARTS)*100+"%".
}

////////////////////////////////////////////////////////////////////////////////////////////////
function falcon_core
{
	AG8 ON. //Disable Lower RCS.
	waitAndDoReEntryburn().
	activateOneEngine().
	aerodynamic_guidance().
	landingBurn(). //3000
	touchdown().
	rocketshutdown().
	after_landing().
}

////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION:
////////////////////////////////////////////////////////////////////////////////////////////////
function main_falcon_return 
{
	CLEARSCREEN.
	update_phase_title("(Return Home)",0).

	//Back to 100% now:
	if CORE:BOOTFILENAME:FIND("boot-boosters.ks") > -1
	{
		update_phase_title("(THRUST MAX)  ", 0, true).
		engines_thrustlimit_to (100).	
	}

	//Activate 3 engines:
	update_phase_title("(ACTIVATE 3 ENGINES)", 0, true).
	activate3engines().
	
	PRINT_STATUS (3). //steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset). // Calculate: impactDist
	if (impactDist > 5000) and (SHIP:altitude > 50000) 
	{
		if STAGE_1_TYPE = "CORE"
			boostback_burn(true).
		else{
			boostback_burn().
		}
		
		activateMainVessel(). //ST-2 or crew dragon
	}
		
	falcon_core ().
}