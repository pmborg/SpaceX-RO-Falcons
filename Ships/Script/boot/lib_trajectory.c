// --------------------------------------------------------------------------------------------
// Filename: lib_trajectory.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Adapted code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Original code:   - https://github.com/AlexAscherson/Kerbal-Kos-Programming/blob/master/old/r1.ks
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              	- Used to calculate orbit position to do a hohmann transfer orbit in
//                    missions (Beyond Earth)
// 01/Nov/2021
// --------------------------------------------------------------------------------------------

function get_rendevous_nodes
{
  // You start at point A, your target is somewhere along its orbit at point B and since you're using a Hohmann transfer your intercept point is at C. Your initial orbit has radius r1 = (6378.1 + 500) km = 6878.1 km, while the target's has radius r2 = (6378.1 + 700) km = 7078.1 km.
  until false {

    WAIT 1.
    CLEARSCREEN.
    set currentorbitradius to body:radius+ altitude.
    set targetorbitradius to body:radius + target:altitude.

    //Transfer Time - Both Methods work:
    set transfertime to constant():pi * sqrt( (currentorbitradius+targetorbitradius)^3/(8*body:mu) ).  //Standard hoffman transfer.
	if transfertime > 60 {
		PRINT "Transfer Time: "+ (transfertime/60) +" mins".
	} else {
		PRINT "Transfer Time: "+ round(transfertime) +" s".
	}.
    //mu = 3.986e5 km^3/s^2 (notice I'm using km for all distances; one must be careful that the units match), we get:

    //Transfer Time via SMA:
    set sma_transfer to (currentorbitradius + targetorbitradius)/2.
    set transfertime1 to 2 * constant():pi * sqrt(sma_transfer^3/body:mu).
    PRINT "T+" + round(missiontime,1) + " Hohmann apoapsis: " + round(targetorbitradius/1000,1) + "km, transfer time: " + round(transfertime1/120,1) + "min".

    // Now, the crucial part for the rendezvous to work is to start the transfer at the right time so that when you reach point C the target is also there. 
    //We know that in time tH you travel 180Ã‚Â°, but how much (in terms of angle) does the target travel? Since the orbit is circular, this is easy to compute:
    set theta to sqrt(body:mu/targetorbitradius^3) * transfertime. //
    //PRINT "theta(degrees target will move during transfer): " +round(theta,1)+ " Rad".
    PRINT "theta (deg): " + round(theta*(180/constant():pi),1)+" deg".

    // The correct phase angle for the rendezvous, phi, is then the difference between the angle you travel, 180Ã‚Â°, and the angle the target travels:
    set phi to constant():pi - theta.
    //PRINT "Phase angle needed for rendevous(phi) in rad: " + round(phi,1) +" rad".
    set phi0 to constant():pi * (1 - sqrt((1+currentorbitradius/targetorbitradius)^3 / 8) ).
    PRINT "Safety check: phi0: "+ round(phi0,1).

    PRINT "Phase angle needed for rendevous-phi:" + round(phi*(180/constant():pi),1) +" deg". // This means that the target must be phi degrees ahead of you when you start the transfer if you are to intercept it at C.

    set phaseanglerateofchange to sqrt(body:mu/targetorbitradius^3) - sqrt(body:mu/currentorbitradius^3).
    PRINT "phase angle ro change: "+ round(phaseanglerateofchange,1)+ " rad/s".
    PRINT "phase angle ro change: "+ round(57.29577951308*phaseanglerateofchange,1)+ " deg/s".

    //Get current Angle on target
    set Angle1 to obt:lan+obt:argumentofperiapsis+obt:trueanomaly. //the ships angle to universal reference direction.
    set Angle2 to target:obt:lan+target:obt:argumentofperiapsis+target:obt:trueanomaly. //target angle
    lock Angle3 to Angle2 - Angle1.
    lock Angle4 to Angle3 - 360 * floor(Angle3/360). 
    //this used to be set to angel 3 as well in case of bugs.
    PRINT "current angle to target: "+ round(angle4,1).


    // current target angular position 
    //PRINT "//////NEW BLOCK:///".
    set positiontarget to target:position - body:position.
    set positionlocal to V(0,0,0) - body:position.
    set targetangularpostioncurrent to arctan2(positiontarget:x,positiontarget:z).
    PRINT "target angular position 1: "+ round(targetangularpostioncurrent,1).
    // target angular position after transfer
    set target_sma to target:obt:semimajoraxis.                       			// for circular orbits
    set orbitalperiodtarget to 2 * constant():pi * sqrt(target_sma^3/body:mu).  // mun/minmus orbital period
    set sma_ship to obt:semimajoraxis.                      
    set orbitalperiodship to 2 * constant():pi * sqrt(sma_ship^3/body:mu).      // ship orbital period

    set transferangle to (transfertime1/2) / orbitalperiodtarget * 360. 
    PRINT "TRANSFER ANGLE V2: " + round(transferangle,1).           // mun/minmus angle for hohmann transfer
    set das to (orbitalperiodship/2) / orbitalperiodtarget * 360.           // half a ship orbit to reduce max error to half orbital period

    set at1 to targetangularpostioncurrent - das - transferangle.                // assume counterclockwise orbits

    PRINT "T+" + round(missiontime,1) + " " + target:name + ", orbital period: " + round(orbitalperiodtarget/60,1) + "min".
    PRINT "T+" + round(missiontime,1) + " | now: " + round(targetangularpostioncurrent,1) + "', xfer: " + round(transferangle,1) + "', rdvz: " + round(at1,1) + "'".


    //number of angles thatneed to pass till we get to the right one
    lock anglesleft to Angle4 - (phi*(180/constant():pi)).
    PRINT "ANGLES LEFT: " + round(anglesleft,1).

    //Time till burn window
    set tb to ((Angle4-(phi*(180/constant():pi)))/(57.29577951308*phaseanglerateofchange)). //seconds till window
    PRINT round(tb,1) + " seconds to burn window".

    // eta to maneuver node
    // current ship angular position 
    lock shipangularpostion_current to arctan2(positionlocal:x,positionlocal:z).
    // ship angular position for maneuver
    set shipangularpostion_manuever_temp to mod(at1 + 180, 360).
    set shipangularpostion_manuever to shipangularpostion_manuever_temp.
    until shipangularpostion_current > shipangularpostion_manuever { set shipangularpostion_manuever to shipangularpostion_manuever - 360. }
    set etanode to (shipangularpostion_current - shipangularpostion_manuever) / 360 * orbitalperiodship.
    PRINT "Time till window2:" + round(etanode,1).

    // Finally, the delta-v requirement is simply that of the Hohmann transfer:
    set dv1 to sqrt(body:mu/currentorbitradius) * (sqrt( 2*targetorbitradius/(currentorbitradius+targetorbitradius) ) - 1).
    PRINT "Dv for transfer: "+ round(dv1,1) + " m/s".
    set dv2 to sqrt(body:mu/targetorbitradius) * (1 - sqrt( 2*currentorbitradius/(currentorbitradius+targetorbitradius) )).
    PRINT "DV to match speed: "+ round(dv2,1) + "m/s".

    //Ship obital period 
    set positionlocal to V(0,0,0) - body:position.
    set sma_ship to positionlocal:mag.                       
    set orbitalperiodship to 2 * constant():pi * sqrt(sma_ship^3/body:mu).
    PRINT "orbitalperiodship: "+ round(orbitalperiodship/60,1) +" mins".


    //PRINT "".
    PRINT "---Transfer Time  : "+ round(transfertime/60,1)+" mins" + round(transfertime,1) + "s".
    PRINT "---Transfer Time 2: " + round(transfertime1/120,1) + "min".

    PRINT "theta1-Target will move during transfer: "+ round(theta*(180/constant():pi),1) +" deg".

    PRINT "Phi: -Phase angle needed for rendevous: " + round(phi*(180/constant():pi),1) +" deg". 
    PRINT "Phi2:-Phase angle needed for rendevous: " + round(transferangle,1)+" deg". 

    PRINT "current angle to target: "+ round(angle4,1).

    PRINT "Time till window1: "+ round(tb,1) + " seconds".
    PRINT "Time till window2: "+ round(etanode,1).

    //the time to burn is the current time + the time to the correct phase angle.  Travel time needs to be equal to the half of the targets period, - the travel time.
    PRINT "Our current angular position: " + round(shipangularpostion_current,1).
    PRINT "Position we need to be when we calculate: "+ round(Angle4-( phi*(180/constant():pi)),1).
    //set warp to 5.
    set angles_per_pecond to 360/ship:orbit:period.
    lock angle_to_intercept_point to shipangularpostion_current -  (anglesleft).
    set seconds_to_intercept_point to shipangularpostion_current -  anglesleft /angles_per_pecond.
    PRINT "angles_per_pecond"+ round(angles_per_pecond,1).
    PRINT "seconds_to_intercept_point:"+ round(seconds_to_intercept_point,1).
    PRINT "#angle_to_intercept_point#"+ round(angle_to_intercept_point,1).
      //CLEARSCREEN.
      PRINT "condition now" + (abs(abs(arctan2(positionlocal:x,positionlocal:z) - (angle4-( phi*(180/constant():pi)))))).
      //intercept angle would be 45 or 225?
      //print+"condition 2: is now right time?"+ round(abs(225 - abs(shipangularpostion_current)),1).
	  
	  set condition to 0.
      if abs(225 - abs(shipangularpostion_current)) > 1 or(45 - abs(shipangularpostion_current))< 1{
		  PRINT "location is right-1".
          set condition to 1.
      }
      //or
      //PRINT "condition 1: is now right time?"+ (anglesleft-abs(angle_to_intercept_point)).
      if (anglesleft-abs(angle_to_intercept_point)) > 44.9 and (anglesleft -abs(angle_to_intercept_point))< 45.9{
         PRINT "location is right-2".
         set condition to 2.
      }
		
	  if angle_to_intercept_point < 10 {
        PRINT "condition true" + angle_to_intercept_point.

        if angle_to_intercept_point < 0.5 {
			PRINT "condition true" + angle_to_intercept_point.
			PRINT "location is right-3".
			set warp to 0.
			WAIT 0.
			break.
        }
      }

    }

    //IF we can do this underneatht the ap or after(?) the target pe its accurate?
    set target_periapsis_longitude to target:obt:longitudeofascendingnode + target:obt:argumentofperiapsis.
    PRINT target_periapsis_longitude.
    set nd to node(time:seconds + abs(tb), 0, 0, dv1).
    add nd.

    // set ned2 to node(time:seconds + abs(tb)+(orbit:period/2), 0, 0, dv1).
    // add ned2.

	set warp to 0.
	CLEARSCREEN.	
	PRINT "FOUND!    " at (0, 35).

	set max_acc to ship:maxthrust/ship:mass.
	print "maxthrust: " + round(maxthrust) + " ".
	print "ship:mass: " + round(ship:mass) + " ".
	print "max_acc: " + round(max_acc) + " ".
	print "deltav: " + round(nd:deltav:mag) + " ".
	if max_acc > 0
		set burn_duration to nd:deltav:mag/max_acc.
	else
		set burn_duration to 300.
	print "Crude Estimated burn duration: " + round(burn_duration) + "s".

	SET MAPVIEW TO FALSE. wait 1. // map view: off
	UNLOCK STEERING. wait 1.
	SAS ON. wait 1.
	if MAPVIEW
		SET MAPVIEW TO FALSE. wait 2. // map view: off
	set sasmode TO "PROGRADE". wait 1.
	//LOCK STEERING TO PROGRADE. wait 0.1.

	set warp to 2.
	until false {
		if (nd:ETA <= (burn_duration/2) + 60 + 15)
		{
			set warp to 0.
			break.
		}.
		PRINT "nd:ETA "+ROUND(nd:ETA) + "        " at (0, 35).
	}.
	
	return nd.
}