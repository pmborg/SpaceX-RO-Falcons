// --------------------------------------------------------------------------------------------
// Filename: PhaseI-Docking.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to Dock CMD with LEM
// 07/Jan/2021
// --------------------------------------------------------------------------------------------
Function Translate {
  Parameter SomeVector.
  if SomeVector:mag > 1 {
    set SomeVector to SomeVector:normalized.
  }

  set ship:control:starboard to SomeVector * ship:facing:starvector.
  set ship:control:fore      to SomeVector * ship:facing:forevector.
  set ship:control:top       to SomeVector * ship:facing:topvector.
}

Function KillRelVelRCS {
  Parameter target_vessel.

  local lock RelativeVelocity to (ship:velocity:orbit - target_vessel:velocity:orbit).
  until RelativeVelocity:mag < 0.1 {
    Translate(-1*RelativeVelocity).
  }
  Translate(V(0,0,0)).
}

Function PortGetter {
  Parameter NameOfVessel is ship.
  Parameter PrePickedPort is "none".
  if PrePickedPort = "none" {

    local portlist is list().
    for port in NameOfVessel:partsdubbedpattern("dock"){
      portlist:add(port).
    }

    local PortNumber is 0.
    until false {
      if portlist[PortNumber]:state = "ready" {
        return portlist[PortNumber].
      } else {
        set PortNumber to PortNumber + 1.
        if PortNumber = PortList:length {
          print "ERROR NO PORTS READY TO BE USED".
        }
      }
    }
  } else {
    return PrePickedPort.
  }
}

Function ApproachDockingPort {
  Parameter ShipDockingPort.
  Parameter TargetDockingPort.
  Parameter Distance.
  Parameter Speed.
  Parameter ErrorAllowed is 0.2.

  ShipDockingPort:controlfrom.

  local Lock DistanceInFrontOfPort to TargetDockingPort:portfacing:vector:normalized * Distance.
  local Lock ShipToDIFOP to TargetDockingPort:nodeposition - ShipDockingPort:nodeposition + DistanceInFrontOfPort.
  local Lock RelativeVelocity to ship:velocity:orbit - TargetDockingPort:ship:velocity:orbit.

  until ShipDockingPort:state <> "ready" {
    Translate((ShipToDIFOP:normalized*Speed) - RelativeVelocity).
	
	//DEBUG:
	clearvecdraws().
    vecdraw(TargetDockingPort:position, DistanceInFrontOfPort, RGB(1,0,0), "DistanceInFrontOfPort", 1.0, true, 0.2).
    vecdraw(v(0,0,0), ShipToDIFOP, RGB(0,1,0), "ShipToDIFOP", 1.0, true, 0.2).
	
    local DistanceVector is (TargetDockingPort:nodeposition - ShipDockingPort:nodeposition).
	set error to abs(Distance - DistanceVector:mag).
	print "Distance Error: "+ROUND(error,2)+"  " at (0,15).
	set ang to vang(ShipDockingPort:portfacing:vector, DistanceVector).
	print "Current Angle: "+ROUND(ang,2)+"  " at (0,16).
	if ang < 30
	{
		SAS ON. wait 0.01.
		set sasmode to "TARGET". wait 0.01.
	}
    if (ang < 2) and (error < ErrorAllowed)
      break.
  }
  Translate(v(0,0,0)).
  SAS OFF.
}

lock steering to prograde. wait 0.1.
CLEARSCREEN. print " ". print " ".
update_phase_title("CMD/LEM Docking", 0, true).

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
print "WAIT FOR ATGERT DISTANCE: 100m".
lock RP to ( ship:position - target_vessel:position).

UNTIL NOT HASNODE { REMOVE NEXTNODE. WAIT 0. } //removeAllNodes!

SAS ON. wait 1.
RCS ON.
set Ship_Distance to 24. //25-1
UNTIL (Ship_Distance >= 25)
{
	set Ship_Distance to RP:mag.
	print "TARGET DISTANCE(25): "+ROUND(Ship_Distance,1) at (0,11).
	WAIT 0.25.
	
	//ROTATE (DO 180):
	if (Ship_Distance > 20)
	{
		set sasmode to "TARGET". wait 1.
	}
	
}
//SAS OFF.
RCS OFF.
UNLOCK RP.
UNLOCK STEERING. wait 1.

//KILL SPEED IF NEEDED:
RCS ON. wait 1.
KillRelVelRCS(target_vessel).

//SIDEWAYSAPPROACH:
set ShipDockingPort to PortGetter(SHIP, "none").
set TargetDockingPort to PortGetter(target_vessel, "none").
update_phase_title("Docking 25m", 0, false).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 25, 0.3).
update_phase_title("Docking 10m", 0, false).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 10, 0.3).
update_phase_title("Docking 5m", 0, false).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 5, 0.2).
update_phase_title("Docking 2.5m", 0, false).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 2.5, 0.1).
update_phase_title("Docking...", 0, false).
ApproachDockingPort(ShipDockingPort, TargetDockingPort, 0, 0.1, 0).


LOG "Docked" to dock.txt.
PRINT "ENTER TO PROCEED". set ch to terminal:input:getchar().	//DEBUG