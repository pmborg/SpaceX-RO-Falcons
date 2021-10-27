// --------------------------------------------------------------------------------------------
// Filename: lib_body_travel_data_lanch.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This data is used to start missions (Beyond Earth)
// 27/Nov/2021
// --------------------------------------------------------------------------------------------

// MORE - INFO: "body-data.txt"

// FOR INTER-PLANETARY TRANSFER: ------------------------------------------------------------
declare global phaseAngle to 999. // 999 = For "Mun" and "Minus"
declare global burn_dV to 999.	  // 999 = For "Mun" and "Minus"

// USING DATA FROM: http://ksp.olex.biz/
// Ejection Δv: file:///C:/Users/pedro/Dropbox/Kerbal/boot/ksp-gh-pages/index.html#/Kerbin/100/Duna/100/false/ballistic/false/1/1
if mission_target = "Moho"  { set phaseAngle to -251.79. set burn_dV to (1492). } //#1 retrograde
if mission_target = "Eve"   { set phaseAngle to  -54.13. set burn_dV to (1072). } //#2 retrograde
//#3 "Kerbin"
if mission_target = "Duna"  { set phaseAngle to   44.36. set burn_dV to (1047). } //#4 prograde
if mission_target = "Dres"  { set phaseAngle to   96.58. set burn_dV to (2442). } //#5 prograde
if mission_target = "Jool"  { set phaseAngle to   96.58. set burn_dV to (1993). } //#6 prograde
if mission_target = "Eeloo" { set phaseAngle to  101.42. set burn_dV to (2056). } //#7 prograde

//MOD: "OPM"
if mission_target = "Sarnus"  { set phaseAngle to   141.86. set burn_dV to (2911). } 
if mission_target = "Urlum"   { set phaseAngle to   135.98. set burn_dV to (2734). } 
if mission_target = "Neidon"  { set phaseAngle to   131.18. set burn_dV to (2734). } 
if mission_target = "Plock"   { set phaseAngle to    69.94. set burn_dV to (2999). } 

//MOD: "Extrasolar"
if mission_target = "Valentine" { set phaseAngle to 103.07. set burn_dV to (9999). } 
// -------------------------------------------------------------------------------------------

if RealSolarSystemMod = true
{
	//Mars Caculation Sample:
	//-----------------------
	
	// [1] Calculate KSP_UA:
	set EarthAPOAPSIS to BODY("Earth"):APOAPSIS.
	set EarthPERIAPSIS to BODY("Earth"):PERIAPSIS.
	set KSP_UA to (EarthAPOAPSIS+EarthPERIAPSIS)/2.
	print "Earth APOAPSIS: " + EarthAPOAPSIS.
	print "Earth PERIAPSIS: " + EarthPERIAPSIS.
	print "KSP_UA: " + KSP_UA.	// REAL UA=149,597,871 km

	// [2] Calculate target UA:
	set Target_UA to (mission_target:APOAPSIS+mission_target:PERIAPSIS)/2.

	// [3] Calculate a: semi major axis
	// 2a = r.Earth + r.mars
	// a = 1/2(r.Earth + r.mars)
	set a to 1/2*(KSP_UA/KSP_UA + Target_UA/KSP_UA).
	print "semi major axis: "+a.			//~1.263 AU

	// [4] Kepler’s Third Law: P^2=a^3
	set P to SQRT(a ^ 3).					//~1.42 years
	print "period of this Hohmann transfer: " + P.

	// [5] Travel to Mars encompasses half of one orbit, so:
	set travel_time to (P/2) * BODY("Earth"):ORBIT:PERIOD/60/60/24.
	print "travel_time: " + travel_time. 	//259.25 days

	// [6] Mars completes one revolution around the sun (360 degrees) in:
	set TargetSunrevolution to mission_target:ORBIT:PERIOD/60/60/24.
	print mission_target:NAME + "Sunrevolution: "+TargetSunrevolution.	// ~687 days

	// [7] Calculate degrees per day:
	set moves_degrees_per_day to 360/TargetSunrevolution.	// 0.524 degrees per day
	print mission_target:NAME + " moves degrees/day: "+moves_degrees_per_day.

	// [8] Calculate Target Move:
	set Target_Move to (moves_degrees_per_day * travel_time).	// ~135.85 degrees
	print mission_target:NAME+" will move: "+Target_Move+" degrees ".

	// [9] FINALLY! Calculate the position of Target at the time of launch:
	set phaseAngle to (180-Target_Move).
	print "phaseAngle: " + phaseAngle.		//44 degrees
	
	wait 2.
}