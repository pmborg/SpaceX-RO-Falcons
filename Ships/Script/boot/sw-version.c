// --------------------------------------------------------------------------------------------
// Filename: sw-version.cpp
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used to specify the SW version and the diferent types of profiles supported.
// 16/Nov/2020
// --------------------------------------------------------------------------------------------

PRINT " ".PRINT " ".PRINT " ".PRINT " ".
//             #.YY.MM.DD
PRINT "SW-Ver: 1.20.11.16" at (0,2).
PRINT time:calendar + " " + time:clock at (23,2).

if SHIP:NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5"
	declare global vehicle_type to "F9v1.2B5".				// BASE: Falcon-9 v1.2Blk:5 
else
if SHIP:NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5 Crew Dragon 2"
	declare global vehicle_type to "Crew Dragon 2".			// BASE: Falcon-9 v1.2Blk:5 
else
if SHIP:NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5"
	declare global vehicle_type to "Falcon Heavy".			// BASE: Falcon-9 v1.2Blk:5 
else
	declare global vehicle_type to "Stage-1".

declare global STAGE_1_TYPE to "".

//PRINT core:BOOTFILENAME.
if CORE:BOOTFILENAME:FIND("boot-boosters-L.ks") > -1 		// STAGE-1L	if vehicle_type = "Falcon Heavy" 
	set STAGE_1_TYPE to "SLAVE".
else if CORE:BOOTFILENAME:FIND("boot-boosters-R.ks") > -1 	// STAGE-1R	if vehicle_type = "Falcon Heavy" 
	set STAGE_1_TYPE to "MASTER".
else if CORE:BOOTFILENAME:FIND("boot.ks") > -1 				// STAGE-2
	set STAGE_1_TYPE to "MAIN".
else if CORE:BOOTFILENAME:FIND("boot-boosters.ks") > -1 	// STAGE-1C
	set STAGE_1_TYPE to "CORE".

PRINT "INIT CODE FOR: "+STAGE_1_TYPE.

// Defaults for Fligth Profile:
if vehicle_type = "Falcon Heavy" 
{
	// Data from: FH Demo 1:
	//declare global Qmax 	to .5*0.40331*(337.367^2).	// 1/2 x air density ρ | Vehicle's speed v^2
	declare global Qmax 	to 15238/1.1.//.5*0.36075*(249.8^2).	// 1/2 x air density ρ | Vehicle's speed v^2
	
	declare global MECO1 	to 1650^2. //5 before: 1675^2.
	declare global SEP   	to 1741^2.
	declare global MECO2 	to 3060^2.
	declare global FAIRSEP  to 160.
	declare global SECO1 	to 7451^2.
}else
if vehicle_type = "Crew Dragon 2"
{
	// Data from: Crew Demo 1:
	//declare global Qmax 	to .5*0.5486668*(282.5^2).	// 1/2 x air density ρ | Vehicle's speed v^2
	declare global Qmax 	to .5*0.34876*(259.9^2).	// 1/2 x air density ρ | Vehicle's speed v^2
	
	declare global MECO1 	to 1875^2.
	declare global FAIRSEP 	to 0.
	declare global SECO1 	to 7503^2.
}else
if vehicle_type = "F9v1.2B5"
{
	// Data from: SpaceX CRS-16:
	//declare global Qmax 	to .5*0.330856*(374.166^2).	// 1/2 x air density ρ | Vehicle's speed v^2
	declare global Qmax 	to .5*0.37435*(295.5^2).	// 1/2 x air density ρ | Vehicle's speed v^2
	
	declare global MECO1 	to 2100^2. //2180^2.
	declare global FAIRSEP 	to 103.
	declare global SECO1 	to 7535^2.
}
else
{
	declare global Qmax 	to .5*1.2*(104.1^2). 		// 1/2 x air density ρ | Vehicle's speed v^2 (Default value)	
	declare global MECO1 	to 2100^2.
	declare global FAIRSEP 	to 160.
	declare global SECO1 	to 7500^2.
}

declare global COM_thrust to 0.
declare global COM_steeringDir to 0.
declare global COM_pitch to 0.
declare global COM_ADDONS_TR_IMPACTPOS to SHIP:GEOPOSITION.
declare global COM_altitude to 0.
//declare global COM_impactDist to 0.
//declare global COM_targetDir to 0.

runpath("boot/useful-tools.c").

// SET Impact_Position TO ADDONS:TR:IMPACTPOS.

//2D Positions:
// SET GroundPos_Target TO LandingPad:POSITION.
// SET GroundPos_Impact TO Impact_Position:POSITION.
// SET GroundPos_Ship TO SHIP:GEOPOSITION:POSITION.

//2D Distances:
// SET Distance_ShipTarget TO (GroundPos_Ship-GroundPos_Target):MAG.
// SET Distance_ShipImpact TO (GroundPos_Ship-GroundPos_Impact):MAG.
// SET Distance_ImpactTarget TO (GroundPos_Target-GroundPos_Impact):MAG.

//Angles:
// SET Angle_Target TO LandingPad:HEADING.
// SET Angle_Impact TO Impact_Position:HEADING.
// SET HeadingDiff_TargImpact TO ABS(headingDifference(Angle_Target,Angle_Impact)).

//TRIG FUNCTIONS:
// SET Angle_ShipTargImpact1 TO (Distance_ShipImpact*(SIN(HeadingDiff_TargImpact)))/Distance_ImpactTarget.
// IF Angle_ShipTargImpact1 > 1 {SET Angle_ShipTargImpact1 TO 1.}//prevents 0 devisors in arcsin
// IF Angle_ShipTargImpact1 < -1 {SET Angle_ShipTargImpact1 TO -1.}//prevents 0 devisors in arcsin
// SET Angle_ShipTargImpact TO ARCSIN(Angle_ShipTargImpact1).

//LATERAL / LONGITUDINAL OFFSET DISTANCES:
// SET HorizontalError TO SIN(Angle_ShipTargImpact)*Distance_ImpactTarget.
// SET LongitudeError TO COS(Angle_ShipTargImpact)*Distance_ImpactTarget.

declare global r to 0.
declare global GM to 0.
declare global g to 0.


