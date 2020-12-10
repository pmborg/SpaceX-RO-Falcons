// --------------------------------------------------------------------------------------------
// Filename: lib_body_travel_data_return.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This data is used to return from missions (Beyond Earth)
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

// FOR: "PhaseV-Return.ks"
declare global return_dV to 9999.
declare global return_ejection_angle to 45. // default
if body:name = "Mun"   { set return_dV to 286.		set return_ejection_angle to 21.88. }	//#3.1
if body:name = "Minmus"{ set return_dV to 286.		set return_ejection_angle to 21.88. }	//#3.2

// USING DATA FROM: http://ksp.olex.biz
if body:name = "Moho"  { set phaseAngle to 76.05. 	set return_dV to (3203+2366). set return_ejection_angle to 94.21. }	//#1
if body:name = "Eve"   { set phaseAngle to 36.07.	set return_dV to (4573+1377). set return_ejection_angle to 162.8. }	//#2
//#3 "Kerbin"											
if body:name = "Duna"  { set phaseAngle to -75.19.  set return_dV to (1452+605).  set return_ejection_angle to 121.06. }//#4
if body:name = "Dres"  { set phaseAngle to -329.68. set return_dV to (1625+1324). set return_ejection_angle to 92.03.  }//#5
if body:name = "Jool"  { set phaseAngle to -48.65.	set return_dV to (9772+2966). set return_ejection_angle to 160.38. }//#6

//MOD: OPM (rescue Eeloo to original position)
//D:\SteamLibrary\steamapps\common\Kerbal Space Program\GameData\OPM\KopernicusConfigs\SarnusMoons\_Eeloo.cfg_
if body:name = "Eeloo" { set phaseAngle to -80.33.	set return_dV to (1888+1398). set return_ejection_angle to 94.14.  }//#7

//MOD: "OPM"
if mission_target = "Sarnus" { set phaseAngle to -9.87.   set return_dV to (2914).  set return_ejection_angle to 136. } //retrograde
if mission_target = "Urlum"  { set phaseAngle to -200.84. set return_dV to (2887).  set return_ejection_angle to 119. } //retrograde
if mission_target = "Neidon" { set phaseAngle to -212.90. set return_dV to (2989).  set return_ejection_angle to 136. } //retrograde
if mission_target = "Plock"  { set phaseAngle to -130.04. set return_dV to (3735).  set return_ejection_angle to 73. } 	//retrograde

//MOD: "Extrasolar"
if mission_target = "Valentine" { set phaseAngle to -130.04.  set return_dV to (3735).  set return_ejection_angle to 73. } //retrograde

// --------------------------------------------------------------------------------------------
if (BODY:name <> DEFAULT_KSC) and (CrewOnBoard)
{
	if ship:Orbit:TRANSITION <> "ESCAPE" {
		if KUniverse:ActiveVessel = SHIP
			set TARGET to BODY(DEFAULT_KSC).
		set target_distance to ROUND(TARGET:ALTITUDEOF(SHIP:POSITION)/1000).

		PRINT "SHIP:name: " +SHIP:name.
		PRINT "Vehicle_type: " + vehicle_type.
		PRINT "Target: " +mission_target.
		PRINT "Distance: "+ target_distance +" km".
		PRINT " ".
		traversePartsForResources("fuel").
		PRINT " ".
	} else
		if KUniverse:ActiveVessel = SHIP
			set TARGET to VESSEL("Refuel_Base"). //As an alternative a Kerbin "orbit Ship"!
	
	WAIT 1.
}