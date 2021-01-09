// --------------------------------------------------------------------------------------------
// Filename: PhaseI-Docking.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Dock (SLM with LEM) or (CD2 with ISS)
// 09/Jan/2021
// --------------------------------------------------------------------------------------------


lock steering to prograde. wait 0.1.
CLEARSCREEN. print " ". print " ".
update_phase_title("SLM/LEM Docking", 1, true).

//STAGE:
if vehicle_type = "SaturnV"
{
	PRINT "Press: y to to confirm the docking stage?". 
	PRINT "Press: n to SKIT IT".
	set ch to terminal:input:getchar(). print "selected: "+ch.
	if (ch = "y" OR ch = "Y")
		{ 
			//[F9]:SEND PROCESSOR ID TO ST-3
			IF PROCESSOR_STAGE3:CONNECTION:SENDMESSAGE(127) //127 = Boost wake-up!
			{
			  PRINT "PROCESSOR_STAGE3: Message sent!".
			  WAIT 1.
			}	
			STAGE. wait 1. 
			STAGE. 
		}
}

//GET LEM VESSEL:
if vehicle_type = "SaturnV"
	set target_vessel to getNearbyProbe(0, "Ship", 100).
	
print target_vessel:name.
set TARGET to target_vessel. wait 1.

//GET SAFETY DISATANCE & ROTATE (DO 180):
lock RP to ( ship:position - target_vessel:position).

UNTIL NOT HASNODE { REMOVE NEXTNODE. WAIT 0. } //removeAllNodes!

SAS ON. wait 1.
RCS ON.
set Ship_Distance to 24. //25-1
UNTIL (Ship_Distance >= 25)
{
	set Ship_Distance to RP:mag.
	print "TARGET DISTANCE(25): "+ROUND(Ship_Distance,1)+"  " at (0,12).
	WAIT 0.25.
	
	//ROTATE (DO 180):
	if (Ship_Distance > 20)
	{
		set sasmode to "TARGET". wait 1.
	}
	
}
RCS OFF.
UNLOCK RP.
UNLOCK STEERING. wait 1.

//KILL REL. SPEED:
update_phase_title("KillRelVel", 0, true).
RCS ON. wait 1.
KillRelVelRCS(target_vessel).

//SIDEWAYSAPPROACH:
set ShipDockingPort to PortGetter(SHIP, "none").
set TargetDockingPort to PortGetter(target_vessel, "none").

update_phase_title("Docking 25m", 0, true).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 25, 0.3).
update_phase_title("KillRelVel", 0, true).
KillRelVelRCS(target_vessel).

update_phase_title("Docking 10m", 0, true).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 10, 0.3).

update_phase_title("Docking 5m", 0, true).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 5, 0.2).

update_phase_title("Docking 2.5m", 0, true).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 2.5, 0.1).

update_phase_title("Docking...", 1, true).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 0, 0.1, 0).

//DONE!
WAIT 5.
LOG "Docked" to dock.txt.
CLEARSCREEN.
update_phase_title("Docking Done", 1, true).
PRINT "ENTER TO PROCEED". set ch to terminal:input:getchar().	//DEBUG