// --------------------------------------------------------------------------------------------
// Filename: lib_body_travel_data_lanch.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This data is used to start missions (Beyond Earth)
// 14/Nov/2020
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