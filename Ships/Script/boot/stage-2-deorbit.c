// --------------------------------------------------------------------------------------------
// Filename: stage-2-deorbit.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Deorbit ST-2
// 19/Dez/2020
// --------------------------------------------------------------------------------------------
CLEARSCREEN.
RUNPATH( "boot/sw-version.c" ).
print "".print "".print "".

function STAGE2_DEORBIT
{ 
	parameter y to 6.
	parameter deorbit_lng to 100.
	
	UNTIL (STAGE:NUMBER = 0)
	{
		STAGE.
		WAIT 1.
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
	print "[".print " ".
	WAIT 10. print "3".
	WAIT 10. print "2".
	WAIT 10. print "1".
	print "Deorbiting...".
	lock throttle to thrust.
	set thrust to 0.5.
	until periapsis < 50000 WAIT 0.1.
	set thrust to 0.
	RCS OFF.
	WAIT 1.
	print " ".
}

update_phase_title("STAGE 2 - Loading...", 0, false).
WAIT 5.

update_phase_title("STAGE 2 - W8-4-SEP", 0, false).
WAIT UNTIL STAGE:NUMBER < 2.

update_phase_title("STAGE 2 - RETROGRADE", 0, false).
SAS OFF.
RCS ON.
LOCK STEERING TO SHIP:RETROGRADE.//  + R(0,0,180).

PRINT "Confirm: - Deorbit ST-2 (y/n)". 
set ch to terminal:input:getchar(). PRINT "selected: "+ch.
if (ch = "y" OR ch = "Y")  
	STAGE2_DEORBIT().


PRINT "Thankyou ST-2!". 

set warp to 2.
WAIT 1.

// KEEP: STEERING
until FALSE WAIT 1.