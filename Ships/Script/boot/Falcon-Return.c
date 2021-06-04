// --------------------------------------------------------------------------------------------
// Filename: Falcon-Return.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              	- Land the Falcon(s) ST-1
// 03/Jun/2021
// --------------------------------------------------------------------------------------------

// --------------
// ACTION GROUPS:
// --------------
// AG2 - Activate F9 (3 engines, for landing)
// AG3 - Faring SEP...
// AG4 - Activate client solar painels
// AG8 - Toggle RCS for landing

// --------------------------------------------------------------------------------------------
// INIT:
// --------------------------------------------------------------------------------------------
SET thrust TO 0.
if vehicle_type <> "SN9-Profile1"
	lock throttle to thrust.

set TOTAL_PARTS to 0.
FOR P IN SHIP:PARTS
SET TOTAL_PARTS to TOTAL_PARTS + 1.

// --------------------------------------------------------------------------------------------
function boostback_burn
{
	parameter do_reverse to false.
	parameter do_reverse_max_speed is 100.
	parameter slowdown is 10000.
	
	if vehicle_type = "SS-BN"
	{
		set tfactor to 0.15.
		set slowdown to 15000.
	} else
		set tfactor to 1.
	
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
 		SET prev_impactDist to impactDist.

		if STAGE_1_TYPE = "CORE"
		{
			set steeringDir to LandingTarget:HEADING.
			set steeringVdeg to 0.
			set steeringVroll to -270.
			//LOCK STEERING TO HEADING(LandingTarget:HEADING,0, -270).
			SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
		} else
			steerToTarget(0, coreAdjustLatOffset, coreAdjustLngOffset, do_reverse).
			
		PRINT_STATUS (3). //impactDist
		
		if vehicle_type <> "SN9-Profile1"
		{
			if(impactDist < 5000)
			{
				PRINT "OP3: impactDist < 3km   " at (0,2).
					SET thrust TO tfactor*0.025.	// 0.0025. Near? for high precision, do it in lower thrust
				RCS ON.
				if STAGE_1_TYPE = "SLAVE" and impactDist < 2500
				{
					SET thrust TO 0.
					LOG  "SLAVE:[we_are_done] " to LOG.txt.
					set we_are_done to TRUE.
				}
			}else if(impactDist < slowdown){
				PRINT "OP2: impactDist < 10km  " at (0,2).
				SET thrust TO tfactor*0.1.
			}else{
				PRINT "OP1: impactDist > 10km   " at (0,2).
				SET thrust TO 1.	// Faraway? all seconds count do it ASAP
			}

			if (do_reverse)
			{
				if (impactDist < 1000) and SHIP:GROUNDSPEED < do_reverse_max_speed or (SHIP:GROUNDSPEED < 350 and impactDist > prev_impactDist)
					break.
			}else{
				if impactDist > prev_impactDist and impactDist < 10000
					break.
			}
		} else {
			if impactDist > prev_impactDist and impactDist < 400
			{
				SET thrust TO 0.
				LOG  "SS:[we_are_done] " to LOG.txt.
				set we_are_done to TRUE.
				break.			
			}
		}
	}
	
	PRINT_STATUS (3). // Send MSG: STOP to "SLAVE" (if MASTER)
	SET SHIP:CONTROL:PILOTMAINTHROTTLE TO 0.	//Pilot Throttle Lever
	SET thrust TO 0.
	wait 1.
	
	if STAGE_1_TYPE = "SLAVE" and SLAVE_STAGE = 0
	{
		LOG  "SET SLAVE_STAGE to 1." to SLAVE.TXT.
		LOG  "REBOOT:1" to LOG.TXT.
		reboot.
	}

	update_phase_title("BOOSTBACK BURN END", 0, false).
	LOG  STAGE_1_TYPE + " " + we_are_done + " - boostback_burn() - END" to LOG.txt.
}

// --------------------------------------------------------------------------------------------
function ReEntryburn
{
	parameter safe_alt.
	parameter safe_power to 0.9.
	parameter maxDescendSpeed to -200.	//Due Terminal Speed don't use another above value.
	
	LOG  "safe_alt: "+safe_alt to LOG.txt.
	
	update_phase_title("WAIT4RE-ENTRY BURN", 1).
	set x to 0.
	
	sas off.
	lock steering to retrograde.
	until SHIP:ALTITUDE < 3000
	{
		SET prev_impactDist to impactDist.
		updateHoverSteering().	
		
		if SHIP:ALTITUDE < safe_alt
		{
			//REENTRY BURN!!!
			if  x = 0 
			{
				update_phase_title("REENTRY BURN", 1).
				set warp to 0.
				set x to 1.
				RCS ON.
				SAS OFF.
				
				LOCK STEERING TO HEADING(steeringDir, steeringVdeg, steeringVroll).
				SET thrust to safe_power .
			}
			
			if STAGE1_LAND_ON = "LAND" {
				if impactDist > 2000
					steerToTarget(steeringPitch, 0, 0). 	// Minor Correction
				else
					steerToTarget(85, 0, 0). 				// Minor Correction
			} else {
				if impactDist > 250
				{
					steerToTarget(steeringPitch, 0, 0). 	// Fast Correction
				} else {
					steerToTarget(82.5, 0, 0). 				// Slower Correction
				}
			}
		} 
		// else {
			// lock steering to retrograde.
		// }
		
		PRINT_STATUS (3).
		if SHIP:VERTICALSPEED > maxDescendSpeed and (impactDist > prev_impactDist or STAGE1_LAND_ON = "LAND")
		{
			
			set LAST_TOUCH to false.
			UNTIL LAST_TOUCH
			{
				SET prev_impactDist to impactDist.
				steerToTarget(steeringPitch, 0, 0). // Fast Correction	
				WAIT 0.1.
				PRINT_STATUS (3).
				if impactDist > prev_impactDist
					set LAST_TOUCH to true.
			}
			
			SET thrust to 0.
			LOCK STEERING TO HEADING(270,0, -270).
			SET thrust to 0.01.
			activateOneEngine().			
		
			AG8 OFF. //Re-enable Lower RCS.
			// FOR FH:
			if vehicle_type <> "SS-BN"
			{
				//Add 10 secs of vertical stability after REENTRY BURN
				update_phase_title("(vertical stability)", 0).
				SET thrust to 0.
				RCS ON.
				LOCK STEERING TO up + R(0,0,180). //UP
				FROM {local x is 5.} UNTIL x = 0 STEP {set x to x-1.} DO 
				{
					wait 1.
					PRINT_STATUS (3).
				}
			}
			AG8 ON. //Disable Lower RCS.
			
			break.
		}
	}
	
	SET thrust to 0.
}

// --------------------------------------------------------------------------------------------
function waitAndDoReEntryburn 
{
	update_phase_title("STEERING VERTICALLY", 0).
	
	if altitude > 100000 and VERTICALSPEED > 0
	{
		SAS ON. wait 1.
		SAS OFF.
		RCS ON.
		steering_falcon(90). //LOCK STEERING TO UP + R(0,0,180).
		wait 10.
		
		// Wait until out ATM...
		until (altitude > BODY:ATM:HEIGHT) or (VERTICALSPEED < 0) //BODY:ATM:HEIGHT=140000m (EARTH)
		{
			WAIT 0.1.
			PRINT_STATUS (3).
		}
		
		if SHIP:altitude > BODY:ATM:HEIGHT 
		{
			if STAGE1_LAND_ON <> "LAND"
				RCS OFF.
			wait 1.
		}
				
		set warp to 0.
		wait 1.
		
		if STAGE_1_TYPE = "MASTER"
			SET KUniverse:ACTIVEVESSEL TO SLAVE_BOOSTER.

		// Warp until re-enter in atmosphere:
		update_phase_title("WAIT FOR RE-ENTER", 0).
		set counter to 0.
		until (SHIP:altitude < BODY:ATM:HEIGHT )  //warp = 0
		{
			WAIT 0.1.  // Wait until return again to ATM...
			PRINT_STATUS(3).
			set counter to counter+1.
			if counter > 50
				RCS OFF.
		}
	}

	update_phase_title("FOCUS ON TARGET", 0).
	// UNLOCK STEERING.
	// WAIT 2.
	// SAS OFF.
	// WAIT 2.

	if STAGE_1_TYPE = "SLAVE"
		set LandingTarget TO offline_LandingZone1.

	RCS ON.
	update_phase_title("(GRID FINS DEPLOY)", 0).
	BRAKES ON.

	// Major Correction:
	PRINT_STATUS(3).

	if STAGE1_LAND_ON = "LAND"
	{
		if vehicle_type <> "SS-BN" 
			set burnAlt to 60000.			// F9: Entry Burn Altitude
		else
			set burnAlt to 45000.			// SS-BN: Entry Burn Altitude
		
		ReEntryburn(burnAlt, 1, -630).		// Speed Goal of Entry Burn
	}
	else
	{
		if mass*1000 > 50000
			set burnAlt to 35000.
		else
			set burnAlt to 32000.
		if (SHIP:altitude > burnAlt)
			ReEntryburn(burnAlt).
	}

}

// --------------------------------------------------------------------------------------------
function aerodynamic_guidance
{
	update_phase_title("AERODYNAMIC GUIDANCE", 1).
	if KUniverse:ActiveVessel = SHIP 
		SET NAVMODE TO "SURFACE".

	RCS ON.	wait 1.
	
	set DO_LANDING to false.
	LOCK STEERING TO HEADING(steeringDir, steeringVdeg, steeringVroll).
	until DO_LANDING
	{
		PRINT_STATUS (3).
		
		if SHIP:ALTITUDE > 7000
			set aerodynamic_target to 100.
		else
			set aerodynamic_target to 50.
	
		if (impactDist < aerodynamic_target) and SHIP:ALTITUDE > 3500
		{
			steering_falcon(90). //LOCK STEERING TO UP + R(0,0,180).
		} else 
		{
			SAS OFF.
			updateHoverSteering().
			steerToTarget(steeringPitch, 0, 0, true).
		}
			
		SET impactDist TO horizontalDistance(LATLNG(LandingTarget:LAT, LandingTarget:LNG), ADDONS_TR_IMPACTPOS).
		
		if STAGE1_LAND_ON = "LAND"
		{
			if impactTime <= 15
				set DO_LANDING to TRUE.
		}else{
			if (SHIP:GROUNDSPEED < 1) or (SHIP:ALTITUDE <= (sBurnDist*1.4)) or impactTime <= 15
				set DO_LANDING to TRUE.
		}
	}
}

// --------------------------------------------------------------------------------------------
function landingBurn
{
	parameter landing_burn to 1000.
	parameter maxDescendSpeed TO 125.
	
	update_phase_title("LANDING BURN", 1).
	SAS OFF.
	RCS ON.
	LOCK STEERING TO HEADING(steeringDir, steeringVdeg, steeringVroll).
	until (alt:radar<100 and Verticalspeed=0)
	{
		if vehicle_type = "SS-BN" 
		{
			if altitude > 5000 and altitude <= 7000
				{ engines_thrustlimit_to(9). }
			if altitude <= 5000
				{ engines_thrustlimit_to(6). }
			wait 0.01.
		}
		
		if  vehicle_type <> "SS-BN" {
			if impactDist > 50
			{
				// set error for min. thrust
				steerToTarget(steeringPitch).	//FAST correction
				set maxDescendSpeed to 15.
				set error to 0.825. 			//Keep up @82.5% x g
			} else {
				steerToTarget(80).				//MEDIUM correction
				set maxDescendSpeed to 125.
				set error to 0.65. 				//Keep up @65% x g
			}
		} else
		{
			if impactDist > 50
			{
				// set error for min. thrust
				steerToTarget(steeringPitch).	//FAST correction
				set maxDescendSpeed to 75.
				set error to 0.95. 				
			}
			else
			{
				steerToTarget(90).	//MEDIUM correction
				set maxDescendSpeed to 125.
				set error to 0.85. 				
			}
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


// --------------------------------------------------------------------------------------------
function touchdown 
{
	SAS OFF.
	update_phase_title("TOUCHDOWN",1).
	SET step to TRUE.
	set checkgear to 0.
	LOCK STEERING TO HEADING(steeringDir, steeringVdeg, steeringVroll).
	set t to 1. //init
	
	set final_kiss to 0.01.
	if vehicle_type = "SN9-Profile1"
		set final_kiss to 1.25.
	
	if vehicle_type = "SS-BN" 
	{
		engines_thrustlimit_to(10).
		if altitude < 500
			engines_thrustlimit_to(4).
	}
	
	until (SHIP:STATUS="LANDED" or sBurnDist <= 0.1) and alt:radar < 100
	{
		PRINT_STATUS (3, t). 
	
		if vehicle_type = "SN9-Profile1"
			set ALT_RADAR to (alt:radar-35).
		else
			set ALT_RADAR to (alt:radar-30).
			
		if alt:radar > 650 and (impactDist < 150) and SHIP:GROUNDSPEED < 5 and Verticalspeed > -25 and vehicle_type <> "SS-BN" 
		{
			set thrust to 0.01. // we are too high! we need to gain vertical speed a bit 
			wait until Verticalspeed < -15.
		}
		
		if alt:radar < 250 and checkgear = 0 {
			GEAR ON.
			set checkgear to 1.
		}
			
		if ALT_RADAR < 250 and LandingTarget:DISTANCE > 100
			set rate to steeringPitch.	//FAST correction
		else
			set rate to 80.				//MEDIUM correction

		if GROUNDSPEED < 5 and impactDist < 50
			set rate to 85.				//SLOW correction

		if ALT_RADAR < 30
			set rate to 89.9.			//Almost "zero" correction
			
		updateHoverSteering().
		steerToTarget(rate).

		//Calc Optimized Throttle:
		if vehicle_type <> "SS-BN" 
			set error to 0.75. 	//F9: Keep up @75% x g
		else
			set error to 0.1.	//SS
		if maxthrust = 0
			set t to 1.
		else
			set t to error*((1000*SHIP:MASS*g)/maxthrust)/maxthrust.
		
		setHoverDescendSpeed(final_kiss+(ALT_RADAR/7.5),t).
		
		wait 0.01.
	}
}

// --------------------------------------------------------------------------------------------
function rocketshutdown
{
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.//SAS OFF.
	update_phase_title("SECURE ROCKET",0).
	SET thrust to 0.
	unlock throttle.							//Auto Throttle
	SET SHIP:CONTROL:PILOTMAINTHROTTLE TO 0.	//Pilot Throttle Lever
	shutDownAllEngines().
	if KUniverse:ActiveVessel = SHIP
		set target to BODY("Earth").
	// Final Stability:
	SAS ON.
	WAIT 2.
	SAS OFF.
	RCS OFF.
	BREAKS OFF.
}

// --------------------------------------------------------------------------------------------
function after_landing
{
	CLEARSCREEN.
	set AFTER_LAND_TOTAL_PARTS to 0.
	FOR P IN SHIP:PARTS 
		SET AFTER_LAND_TOTAL_PARTS to AFTER_LAND_TOTAL_PARTS + 1.

	if TOTAL_PARTS = AFTER_LAND_TOTAL_PARTS and STATUS <> "SPLASHED"
		if vehicle_type = "SN9-Profile1"
			PRINT "GRATZ: Another Starship landed safely!".
		else
			PRINT "GRATZ: Another Booster landed safely!".
	else
		PRINT "INTEGRITY: "+(AFTER_LAND_TOTAL_PARTS/TOTAL_PARTS)*100+"%".

	WAIT 5.
}

// --------------------------------------------------------------------------------------------
function guide_falcon_core
{
	AG8 ON. //Disable Lower RCS.
	waitAndDoReEntryburn().
	activateOneEngine().
	//if vehicle_type <> "SS-BN"
	aerodynamic_guidance().
	landingBurn().
	touchdown().
	rocketshutdown().
	after_landing().
}

// --------------------------------------------------------------------------------------------
function prepare_boostback_burn
{
	parameter default_max_distance to 5000. 
	
	PRINT_STATUS (3). // Calculate: impactDist
	if (impactDist > default_max_distance) and (SHIP:altitude > 50000)
	{
		if (STAGE_1_TYPE = "CORE") and (STAGE1_LAND_ON = "SEA")
			boostback_burn(true).
		else{
			boostback_burn().
		}
	}
}	

// --------------------------------------------------------------------------------------------
// MAIN FUNCTION:
// --------------------------------------------------------------------------------------------
function main_falcon_return 
{
	CLEARSCREEN.
	update_phase_title("(Return Home)",0).

	set steeringDir to LandingTarget:HEADING.
	set steeringVdeg to 0.
	set steeringVroll to -270.
	LOCK STEERING TO HEADING(steeringDir,steeringVdeg,steeringVroll).

	//Back to 100% now:
	if CORE:BOOTFILENAME:FIND("boot-boosters.ks") > -1
	{
		update_phase_title("(THRUST MAX)  ", 0, true).
		engines_thrustlimit_to (100).	
	}

	if vehicle_type <> "SS-BN"
	{
		//F9: Activate 3 engines:
		update_phase_title("(ACTIVATE 3 ENGINES)", 0, true).
		activate3engines().
	} else {
		//SS: all stack as 1:
		update_phase_title("(LIM. 40% ENGINES)", 0, true).
		AG2 ON.
		WAIT 0.1.
		engines_thrustlimit_to(40).
		WAIT 0.1.
	}
	
	if NOT EXISTS(STAGE_1_TYPE+"burn.txt")
	{
		prepare_boostback_burn().
		LOG "boostback_burn" to STAGE_1_TYPE+"burn.txt".
	}

	until (altitude > BODY:ATM:HEIGHT) or (VERTICALSPEED < 0) //BODY:ATM:HEIGHT=140000m (EARTH)
	{
		WAIT 0.1.
		PRINT_STATUS (3).
	}
	if vehicle_type <> "SN9-Profile1" 
		activateMainVessel(). //Better switch out of ATM go to ST-2
			
	guide_falcon_core().
	
	activateMainVessel(). //After landing go to ST-2
}