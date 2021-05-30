// --------------------------------------------------------------------------------------------
// Filename: sw-version.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from:  - https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              Used to specify the SW version and the diferent types of profiles supported.
// 30/may/2021
// --------------------------------------------------------------------------------------------
LOG   "START: sw-version.c" to log.txt.
PRINT " ".PRINT " ".PRINT " ".PRINT " ".
//                          #.YY.MM.DD
set version_str to "SW-Ver: 1.21.05.30".
PRINT version_str at (0,2). PRINT time:calendar + " " + time:clock at (23,2).

// REGRESSION TESTS for KOS, Automatic Pilot Orbit and Landing:
// ------------------------------------------------------------
// [ok] F9 QMAX                         1.20.12.27 ("LAND") 1.20.12.28 ("SEA")
// [ok] F9 ST-1 STAGE                   1.20.12.27 ("LAND") 1.20.12.28 ("SEA")
// [ok] F9 ST-1 LANDING                 1.20.12.27 ("LAND") 1.20.12.28 ("SEA")
// [ok] F9 ST-1 FARING RECOVER          1.20.12.27 ("LAND") 1.20.12.28 ("SEA")
// [ok] F9 ST-2 LEO ORBIT               1.20.12.27 ("LAND") 1.20.12.28 ("SEA")
// [ok] F9 ST-2 DE-ORBIT                1.20.12.27 ("LAND") 1.20.12.28 ("SEA")

// [ok] Crew Dragon 2 QMAX              1.21.01.08 ("LAND")
// [ok] Crew Dragon 2 STAGE             1.21.01.08 ("LAND")
// [ok] Crew Dragon 2 ST-1 LANDING      1.21.01.08 ("LAND")
// [ok] Crew Dragon 2 DRAGON LEO ORBIT  1.21.01.08 ("LAND")
// [ok] Crew Dragon 2 DRAGON DE-ORBIT   1.21.01.08 ("LAND")

// [ok] SaturnV ST1 QMAX                    1.21.01.09
// [ok] SaturnV ST1 ST1- STAGE              1.21.01.09
// [ok] SaturnV ST2 LEO ORBIT               1.21.01.09
// [ok] SaturnV ST2 SLE DOCKING             1.21.01.09
// [ok] SaturnV ST3 LUNAR BURN             1.21.01.09
// [ok] SaturnV ST3 EARTH SOI WARP          1.21.01.09
// [ok] SaturnV ST3 MOON PE WARP            1.21.01.09
// [ok] SaturnV MOON CAPTURE                1.21.01.09
// [ok] SaturnV MOON CIRCLE<200km           1.21.01.09
// [ok] SaturnV MOON CIRCLE<114km           1.21.01.09
// [ok] SaturnV SLM (CMD/LEM) UNDOCK        1.21.01.09
// [ok] SaturnV LEM PREPARE FOR LANDING     1.21.01.09
// [ok] SaturnV LEM PREPARE FOR CIRCULARIZE 1.21.01.09
// [ok] SaturnV LEM LUNAR (horizontal burn)...coding...
// [ok] SaturnV LEM LUNAR LANDING          ...coding...

// [ok] FH ST1 QMAX                         1.20.12.07
// [ok] FH ST1 STAGE                        1.20.12.07
// [ok] FH ST1 Master ST-1 LANDING          1.20.11.23
// [ok] FH Core ST-1 LANDING                1.20.12.07
// [ok] FH LEO Orbit                        1.20.11.22
// [ok] FH GSO Orbit                        1.20.12.07



//https://ksp-kos.github.io/KOS/structures/misc/loaddistance.html#attribute:SITUATIONLOADDISTANCE:UNLOAD
//MAGIC LINES! Extend Physics:
SET KUNIVERSE:DEFAULTLOADDISTANCE:FLYING:UNLOAD TO 1960000. 
SET KUNIVERSE:DEFAULTLOADDISTANCE:FLYING:LOAD   TO 1969500.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:FLYING:PACK TO   1969999.
SET KUNIVERSE:DEFAULTLOADDISTANCE:FLYING:UNPACK TO 1969000.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SUBORBITAL:UNLOAD TO 1960000. 
SET KUNIVERSE:DEFAULTLOADDISTANCE:SUBORBITAL:LOAD   TO 1969500.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SUBORBITAL:PACK TO   1969999.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SUBORBITAL:UNPACK TO 1969000.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:ORBIT:UNLOAD TO 1960000. 
SET KUNIVERSE:DEFAULTLOADDISTANCE:ORBIT:LOAD   TO 1969500.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:ORBIT:PACK TO   1969999.
SET KUNIVERSE:DEFAULTLOADDISTANCE:ORBIT:UNPACK TO 1969000.
WAIT 0.001.

SET KUNIVERSE:DEFAULTLOADDISTANCE:LANDED:UNLOAD TO  10000.
SET KUNIVERSE:DEFAULTLOADDISTANCE:LANDED:LOAD TO    09500.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:LANDED:PACK TO    9999.
SET KUNIVERSE:DEFAULTLOADDISTANCE:LANDED:UNPACK TO  9000.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SPLASHED:UNLOAD TO    10000.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SPLASHED:LOAD TO      09500.
WAIT 0.001.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SPLASHED:PACK TO      9999.
SET KUNIVERSE:DEFAULTLOADDISTANCE:SPLASHED:UNPACK TO    9000.
WAIT 0.001.

//This file is to allow Final USER to do some customizations at flight profile:
runpath("boot/USER-CONFIG.KS").

declare global kspver to 0.
if EXISTS("kspver.c")
    runpath ("kspver.c").

// GLOBALS:
// --------------------------------------------------------------------------------------------
declare global r to 0.
declare global GM to 0.
declare global g to 0.

//steering_falcon()
declare global steeringDir to 0.
declare global steeringVdeg to 0.
declare global steeringVroll to 0.

//FH:
declare global COM_thrust to 0.
declare global COM_steeringDir to 0.
declare global COM_pitch to 0.
declare global COM_ADDONS_TR_IMPACTPOS to SHIP:GEOPOSITION.
declare global COM_altitude to 0.

//SHIP NAMES: (For vessel switch)
declare global SHIP_NAME to SHIP:NAME.                              //["NAME"]_probe
declare global MAIN_SHIP_NAME to SHIP_NAME:REPLACE(" probe", "").   //["NAME"]       (Original name)

//DEBUG:
// PRINT "MODEL: "+SHIP_NAME.
// PRINT "BOOT: " + CORE:BOOTFILENAME.
// --------------------------------------------------------------------------------------------

declare global vehicle_company to "SpaceX". 

if SHIP_NAME = "Starship SN20"
{
	declare global vehicle_type to "SN20-Profile".
}
else
if SHIP_NAME = "Starship SN9"
{
	declare global vehicle_type to "SN9-Profile1".
}
else
if SHIP_NAME = "Apollo11-4KSP1.11"
{
    declare global vehicle_type to "SaturnV".               // BASE: Falcon-9 v1.2Blk:5 
    set vehicle_company to "NASA".
}
else
if SHIP_NAME = "PMBT-SpaceX Falcon 1 (Merlin 1A)" or 
   SHIP_NAME = "PMBT-SpaceX Falcon 1 (Merlin 1C)"
    declare global vehicle_type to "F1-M1".                 // BASE: Falcon 1 (Merlin 1A)
else
if SHIP_NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5"
    declare global vehicle_type to "F9v1.2B5".              // BASE: Falcon-9 v1.2Blk:5 
else
if SHIP_NAME = "PMBT-SpaceX Falcon 9 v1.2 Block-5 Crew Dragon 2"
    declare global vehicle_type to "Crew Dragon 2".         // BASE: Falcon-9 v1.2Blk:5 
else
if SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5" or 
   SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM" or
   SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM2"
    declare global vehicle_type to "Falcon Heavy".          // BASE: Falcon-9 v1.2Blk:5 
else
{
    if MASS < 20
        declare global vehicle_type to "Stage-2".
    else
        declare global vehicle_type to "Stage-1".
}

declare global vehicle_sub_type to vehicle_type.

if SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM" or
   SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM2"
    set vehicle_sub_type to "Falcon Heavy LEM".

if CORE:BOOTFILENAME:FIND("boot-fairings.ks") > -1
    set vehicle_type to "Fairing".

//STAGE_1_TYPE:
// boot-boosters-L.ks
// boot-boosters-R.ks
// boot-boosters.ks
// boot-droneship.ks
// boot-fairings.ks
// boot-iss.ks
// boot-landingzone.ks
// boot-st2.ks
// boot.ks
// --------------------------------------------------------------------------------------------
declare global STAGE_1_TYPE to "".
if CORE:BOOTFILENAME:FIND("boot-boosters-L.ks") > -1    // STAGE-1L (if vehicle_type = "Falcon Heavy")
    set STAGE_1_TYPE to "SLAVE".
else 
if CORE:BOOTFILENAME:FIND("boot-boosters-R.ks") > -1    // STAGE-1R (if vehicle_type = "Falcon Heavy")
    set STAGE_1_TYPE to "MASTER".
else 
if CORE:BOOTFILENAME:FIND("boot-boosters.ks") > -1      // STAGE-1 (General ST-1)
    set STAGE_1_TYPE to "CORE".
else 
if CORE:BOOTFILENAME:FIND("boot.ks") > -1               // STAGE-2 (General ST-2)
{
    set STAGE_1_TYPE to "MAIN".
    LOG version_str to LOG.txt.
}

if STAGE_1_TYPE <> ""
{
    PRINT "INIT CODE FOR: "+STAGE_1_TYPE.
    LOG  "["+STAGE_1_TYPE+"] SHIP_NAME: "+MAIN_SHIP_NAME to LOG.txt.
}

// Defaults for Fligth Profile:
// --------------------------------------------------------------------------------------------
if vehicle_type = "SaturnV"
{
    // Data: SaturnV
    declare global Qmax     to 14600/1.1.
    declare global MECO1    to 9999^2.
    declare global FAIRSEP  to 110*1000.
    if KUniverse:ActiveVessel = SHIP
        SET TARGET TO "Moon".   wait 1.
}else
if vehicle_type = "Falcon Heavy" 
{
    // Data from: FH Demo 1:
    declare global Qmax     to 15238/1.1.
    declare global MECO1    to 1545^2.  //1545* (BOOST SEP)
    declare global MECO2    to 2950^2.  //3060. (CORE STAGE)
    declare global FAIRSEP  to 160*1000.//      (FAIRING STAGE)
    
    if vehicle_sub_type = "Falcon Heavy LEM"
    {
        set MECO1 to 9999^2.  //A non recovery Mission 
        set MECO2 to 9999^2.  //A non recovery Mission
    }
}else
if vehicle_type = "Crew Dragon 2"
{
    // Data from: Crew Demo 1:
    declare global Qmax     to .5*0.34876*(259.9^2).
    declare global MECO1    to 1900^2.// 1875*
    declare global FAIRSEP  to 160*1000.
}else
if vehicle_type = "F9v1.2B5"
{
    // Data from: SpaceX CRS-16:
    declare global Qmax     to 16920/1.2.
    if STAGE1_LAND_ON = "LAND"
        declare global MECO1    to 1835^2. //AP:160
    else
        declare global MECO1    to 2080^2. //2180*
    declare global FAIRSEP  to 160*1000.
}else
if vehicle_type = "F1-M1"
{
    declare global Qmax     to 8576/1.1.
        
    if SHIP_NAME = "PMBT-SpaceX Falcon 1 (Merlin 1A)"
        declare global MECO1    to 2300^2.  //Change from: ASCENT to Orbit
    if SHIP_NAME = "PMBT-SpaceX Falcon 1 (Merlin 1C)"
        declare global MECO1    to 3160^2.  //Change from: ASCENT to Orbit
    
    declare global FAIRSEP  to 160*1000.
}
else
{
    declare global Qmax     to .5*1.2*(250^2).      // 1/2 x air density ρ | Vehicle's speed v^2 (Default value)    
    declare global MECO1    to 2100^2.
    declare global FAIRSEP  to BODY:ATM:HEIGHT.
}

if vehicle_type = "SN9-Profile1" 
{
	declare global Qmax to 6200.
}

if  Qmax <> .5*1.2*(250^2)
    LOG "Normal" to normal.txt.

// PROFILE FUNCTIONS: -------------------------------------------------------
function set_max_delta_curve
{
    set e to constant:e.
    set I to altitude/(15000).
    
    if vehicle_type = "SaturnV"
    {
        set delta to (1*(e^I)*(-1))*1.4.    //Rotate 40% Faster
        if delta < (-75)                    //MAX. Keep: 14 deg nose up
            set delta to (-75).
    }
    else
    if vehicle_type = "Falcon Heavy"
    {
        set delta to (1*(e^I)*(-1))*1.2.    //Rotate 20% Faster
        if vehicle_sub_type = "Falcon Heavy"
        {
            if delta < (-60)                //MAX. Keep: 30 deg nose up
                set delta to (-60).
        } 
        else if SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM"
        {
            if delta < (-80)                //MAX. Keep: 10 deg nose up
                set delta to (-80). 
        }
        else if SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM2"
        {
            if delta < (-70)                //MAX. Keep: 20 deg nose up
                set delta to (-70). 
        }
    } else
    if vehicle_type = "F1-M1"
    {
        set delta to (1*(e^I)*(-1))*1.3.        //Rotate 30% Faster
        if delta < (-80)                        //MAX. Keep: 10 deg nose up
            set delta to (-80).
    } else {
        if STAGE1_LAND_ON <> "LAND"
        {
            set delta to (1*(e^I)*(-1)).        //Normal Rotation
            if delta < (-50)                    //MAX. Keep: 40 deg nose up
                set delta to (-50).
        } else {
            set delta to (1*(e^I)*(-1))*1.4.    //Rotate 40% Faster
            if delta < (-70)                    //MAX. Keep: 20 deg nose up
                set delta to (-70).         
        }
    }
    
    return delta.
}

function set_Vdeg 
{
    if vehicle_type = "Crew Dragon 2"
        return 90-74.   // Vertical = 90
    else
    if  SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM" //or SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM2"
        return 90-76.   // Vertical = 90
    else
    if vehicle_type = "Falcon Heavy"
        return 90-85.   // Vertical = 90
    
    return  90-81.  // Vertical = 90
}


// SELECT VEHICLE TYPE: -------------------------------------------------------
declare global orbit_type to "LEO".         // Default: Orbit Type
declare global LEOrbit to 300000.           // Default: Orbit Target at Launch

if vehicle_sub_type = "Falcon Heavy"
    set orbit_type to "GSO".

if SHIP_NAME = "PMBT-SpaceX Falcon Heavy v1.2 Block-5 LEM"
    declare global LEOrbit to 300000.       // Default: Orbit Target at Launch
    
if orbit_type = "GSO" 
    set LEOrbit to 33000000.                // 1st step for (stage-2)


// LOAD TOOLS: -------------------------------------------------------
runpath("boot/useful-tools.c").
LOG   "END: sw-version.c" to log.txt.
LOG   "-----------------" to log.txt.