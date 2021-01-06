// --------------------------------------------------------------------------------------------
// Filename: stage-2-deorbit.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Deorbit ST-2
// 26/Dez/2020
// --------------------------------------------------------------------------------------------
CLEARSCREEN.
RUNPATH( "boot/sw-version.c" ).
set STAGE_1_TYPE to "ST-2".
print "".print "".print "".

update_phase_title("STAGE 2 - Loading...", 0, true).
WAIT 1.

update_phase_title("STAGE 2 - WAIT FOR SEP", 0, true).
WAIT UNTIL NOT CORE:MESSAGES:EMPTY.
SET RECEIVED TO CORE:MESSAGES:POP.

function DEORBIT
{ 
	parameter deorbit_lng to 100.
	parameter deorbit_pe to  50000.
	parameter y to 6.
	
	core:doaction("Open Terminal", true).
	if vehicle_type <> "Crew Dragon 2"
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
	update_phase_title("STAGE 2 - Deorbiting.", 0, true).
	lock throttle to thrust.
	set thrust to 0.5.
	until periapsis < deorbit_pe WAIT 0.1.
	set thrust to 0.
	RCS OFF.
	WAIT 1.
	print " ".
}

wait 5.
core:doaction("Open Terminal", true).
update_phase_title("STAGE 2 - RETROGRADE", 0, true).
SAS OFF. wait 1.
RCS ON. wait 1.
LOCK STEERING TO SHIP:RETROGRADE. wait 1.

if KUniverse:ActiveVessel <> SHIP {
	update_phase_title("(WAIT TO BE ACTIVE)", 0, true).
	UNTIL (KUniverse:ActiveVessel = SHIP) WAIT 1.
}
// PRINT "Confirm: - Deorbit (y/n)". 
// set ch to terminal:input:getchar(). PRINT "selected: "+ch.
// if (ch = "y" OR ch = "Y")  
if vehicle_type <> "Crew Dragon 2"
	DEORBIT(100, 50000).	//ST-2
else
	DEORBIT(+165, 0).		//CD-2 -179

if vehicle_type <> "Crew Dragon 2"
	PRINT "Thankyou ST-2!". 
else
{
	PRINT "Good Luck!". 
	UNTIL (STAGE:NUMBER = 4)
		{ STAGE. WAIT 1.}
}

set warp to 0. WAIT 1.
set kuniverse:timewarp:MODE to "RAILS". WAIT 1.
set warp to 2. WAIT 1. //WARP TO ATM
wait 5.
set kuniverse:timewarp:MODE to "PHYSICS". WAIT 1.
set warp to 3. WAIT 1. //WARP IN ATM

if vehicle_type <> "Crew Dragon 2"
{
	until FALSE WAIT 1.	// KEEP: STEERING
} else {
	wait until altitude < 5000.
	UNTIL (STAGE:NUMBER = 2)
		{ STAGE. WAIT 1.}
	wait until altitude < 1000.
	UNTIL (STAGE:NUMBER = 0)
		{ STAGE. WAIT 1.}
		
	set warp to 3. WAIT 1. //WARP IN ATM
	wait until altitude < 50.
	set warp to 0. WAIT 1.
}