// --------------------------------------------------------------------------------------------
// Filename: stage-3-deorbit.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Deorbit ST-3
// 07/Jan/2020
// --------------------------------------------------------------------------------------------
CLEARSCREEN.
RUNPATH( "boot/sw-version.c" ).
set STAGE_1_TYPE to "ST-3".
print "".print "".print "".

update_phase_title("STAGE 3 - Loading...", 0, true).
WAIT 1.

update_phase_title("STAGE 3 - WAIT FOR SEP", 0, true).
WAIT UNTIL NOT CORE:MESSAGES:EMPTY.
SET RECEIVED TO CORE:MESSAGES:POP.

function DEORBIT
{ 
	parameter deorbit_lng to 100.
	parameter deorbit_pe to  50000.
	parameter y to 6.
	
	core:doaction("Open Terminal", true).
	{
		UNTIL (STAGE:NUMBER = 0)
			{ STAGE. WAIT 1.}
	}
	
	set warp to 2.
	set we_are_done to FALSE.	
	until we_are_done
	{
		PRINT "[SHIP:GEOPOSITION]: "+ROUND (SHIP:GEOPOSITION:LAT,3)+" " +ROUND (SHIP:GEOPOSITION:LNG,3)  at (0,y).
		if SHIP:GEOPOSITION:LNG > deorbit_lng and SHIP:GEOPOSITION:LNG < (deorbit_lng+1)
			set we_are_done to TRUE.
	}
	set warp to 0.
	
	RCS ON.
	LOCK STEERING TO SHIP:RETROGRADE.//  + R(0,0,180).
	print " ".
	WAIT 7.5. print "5".
	WAIT 7.5. print "4".
	WAIT 7.5. print "3".
	WAIT 7.5. print "2".
	WAIT 7.5. print "1".
	update_phase_title("STAGE 3 - Deorbiting.", 0, true).
	lock throttle to thrust.
	set thrust to 0.5.
	until periapsis < deorbit_pe WAIT 0.1.
	set thrust to 0.
	RCS OFF.
	WAIT 1.
	print " ".
}

//wait 5.
core:doaction("Open Terminal", true).
update_phase_title("STAGE 3 - PROGRADE", 0, true).
// SAS OFF. wait 1.
RCS ON. wait 1.
LOCK STEERING TO SHIP:PROGRADE. wait 1.

// if KUniverse:ActiveVessel <> SHIP {
	// update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
	// UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
// }
// DEORBIT(+165, 0).		//CD-2 -179
PRINT "Thankyou ST-3!". 

until FALSE WAIT 1.	// KEEP: STEERING
