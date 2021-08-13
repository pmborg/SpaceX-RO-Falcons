// --------------------------------------------------------------------------------------------
// Filename: atm.cpp
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX-RO-Falcons Mod (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This code is used go get more realistic data from Planet Earth Atmosphere.
// 13/Aug/2021
// --------------------------------------------------------------------------------------------
LOG   "START: atm.c" to LOG_FILE.
// --------------------------------------------------------------------------------------------
// Algorithm-1:
// --------------------------------------------------------------------------------------------
declare global qVal to 0.
declare global machVal to 0.

set constdoubleTABLE0 to list (00000.0  , -0.0065 , 288.150 , 1.01325000000000E+5).
set constdoubleTABLE1 to list (11000.0  ,  0.0000 , 216.650 , 2.26320639734629E+4).
set constdoubleTABLE2 to list (20000.0  ,  0.0010 , 216.650 , 5.47488866967777E+3).
set constdoubleTABLE3 to list (32000.0  ,  0.0028 , 228.650 , 8.68018684755228E+2).
set constdoubleTABLE4 to list (47000.0  ,  0.0000 , 270.650 , 1.10906305554966E+2).
set constdoubleTABLE5 to list (51000.0  , -0.0028 , 270.650 , 6.69388731186873E+1).
set constdoubleTABLE6 to list (71000.0  , -0.0020 , 214.650 , 3.95642042804073E+0).
set constdoubleTABLE7 to list (84852.0  ,  0.0000 , 186.946 , 3.73383589976215E-1).

function TABLE4
{
 	parameter h0.
	parameter x.
	
	if h0 = 0 return constdoubleTABLE0[x].
	if h0 = 1 return constdoubleTABLE1[x].
	if h0 = 2 return constdoubleTABLE2[x].
	if h0 = 3 return constdoubleTABLE3[x].
	if h0 = 4 return constdoubleTABLE4[x].
	if h0 = 5 return constdoubleTABLE5[x].
	if h0 = 6 return constdoubleTABLE6[x].
	if h0 = 7 return constdoubleTABLE7[x].
	
	PRINT "ERROR".
	EXIT.
}

function Temperature				//<==================================TEMPERATURE (K)
{
	parameter z.					//<--------ALTITUDE (m) (T IS VALID FOR -5,000 m < z < 86,000 
	
	set H to z*6356766/(z+6356766).	//..............................[equation18]
	set b2 to 0.
	FROM {local b is 0.} UNTIL b >= 7 STEP {set b to b+1.} DO
	{
		set b2 to b.
		if(H<TABLE4(b+1,0))
			break.
	}
	return TABLE4(b2,2)+TABLE4(b2,1)*(H-TABLE4(b2,0)).
}//K

function Pressure						//<=======================================PRESSURE (Pa)doublez)
{
	parameter z.						//<--------ALTITUDE (m) (P IS VALID FOR -5,000 m < z < 86,000
	
	set H to (z*6356766)/(z+6356766).	//..............................[equation 18]
	set b to 0.
	FROM {local i is 0.} UNTIL i >= 7 STEP {set i to i+1.} DO
	{
		set b to i.
		if(H<TABLE4(b+1,0))
			break.
	}
	set C to -.0341631947363104.		//................C = -G0*M0/RSTAR [pages 8,9,3]
	set Hb to TABLE4(b,0).
	set Lb to TABLE4(b,1).
	set Tb to TABLE4(b,2).
	set Pb to TABLE4(b,3).
	
	if (abs(Lb)>1E-12)
		return Pb*((1+Lb/Tb*(H-Hb)) ^ (C/Lb)).
	else
		return Pb*(constant:e ^ (C*(H-Hb)/Tb)).
}// (Pa)

// Test:
// PRINT 0+": Temperature: "+ROUND (Temperature(0),2) + " Pressure: "+( ROUND (Pressure(0)/100,2))+" (hPa)".
// FROM {local h is 0.} UNTIL h > 100000 STEP {set h to h+10000.} DO
// {
	// PRINT h+": Temperature: "+ROUND (Temperature(h),1) + " Pressure: "+( ROUND (Pressure(h)/100,2))+" (hPa)".
// }

function display_speed_kmh
{
	parameter h0.
	parameter v0.
	parameter indice.
	parameter xpos is 22.
	
	PRINT "Altitude: "		 	at (0,indice+0).	PRINT ROUND(h0/1000,1)+" km   "		 	at (xpos,indice+0).
	PRINT "Speed: "				at (0,indice+1).	PRINT ROUND(v0*3.6,1)+" km/h   "		at (xpos,indice+1).
}

// --------------------------------------------------------------------------------------------
// Algorithm-2:
// --------------------------------------------------------------------------------------------
// US Standard Atmosphere, 1976
function update_atmosphere 
{
	parameter h0.
	parameter v0.
	set indice to 20.
	
	display_speed_kmh (h0, v0, indice).
	
	if ROUND(BODY:ATM:ALTITUDEPRESSURE(h0),4) = 0 and ROUND(BODY:ATM:ALTITUDETEMPERATURE(h0),1) = 0
	{
		PRINT "--- Cº   "		at (22,indice+3).
		PRINT "--- kg/m^3   "	at (22,indice+4).
		PRINT "--- (hPa)      "	at (22,indice+5).
		PRINT "--- m/sec   "	at (22,indice+6).
		PRINT "---   "			at (22,indice+7).
		PRINT "--- N/m^2      "	at (22,indice+8).
		PRINT "--- K     "		at (22,indice+11).
		PRINT ROUND(SHIP:Q,3)+"     " at (22,indice+12).
		return.
	}
	
	set TEMPSL to 518.67.
	set RHOSL to 0.00237689.
	set PRESSSL to 2116.22.
	set saTheta to 1.0.
	set saSigma to 1.0.
	set saDelta to 1.0.
	set h to h0 * 3.28084.	 	// m to ft
	set v to v0 * 3.28084.		// m/s to ft/sec
	if ( h<36089 ) //<11000m (Troposphere)
	{
	   set saTheta to 1.0 - h/145442.
	   set saDelta to (1.0 - h/145442) ^ 5.255876.
	}
	else if ( h<65617 )//<20000m (Stratosphere)
	{
	   set saTheta to 0.751865.
	   set saDelta to 0.223361 * constant:e ^( (36089-h)/20806 ).
	}
	else if ( h<104987 )//<32000m
	{
	   set saTheta to 0.682457 + h/945374.
	   set saDelta to ( (0.988626+h/652600) ^ (-34.16319) ).
	}
	else if ( h<154199 )//<47000m
	{
	   set saTheta to 0.482561 + h/337634.
	   set saDelta to ( (0.898309+h/181373) ^ (-12.20114) ).
	}
	else if ( h<167323 )//<51000m
	{
	   set saTheta to 0.939268.
	   set saDelta to 0.00109456 * constant:e ^ ( (h-154200)/-25992 ).
	}
	else if ( h<232940 )//71000 (Mesosphere)
	{
	   set saTheta to 1.434843 - h/337634.
	   set saDelta to ( (0.838263-h/577922) ^ 12.20114 ).
	}
	
	set saSigma to saDelta / saTheta.
	
	// Calc in US Units:
	set tempVal to TEMPSL * saTheta.	//°R
	set rhoVal to RHOSL * saSigma.
	set pVal to PRESSSL * saDelta.
	set soundVal to SQRT( 1.4*1716.56*(tempVal) ).
	set machVal to v/soundVal.
	set qVal to 0.7*pVal*machVal*machVal.
	
	// Convert to Europe SI:
	set tempVal to tempVal/1.8. 		// deg K
	set tempVal to tempVal - 273.15.	// deg C
	set rhoVal to rhoVal / .068521  / .028317.
	set pVal to pVal / .020885 / 100. 	//hPa
	set soundVal to soundVal/3.28084.
	set qVal to qVal / .020885.

	// Algorithm-1:
	// ------------
	PRINT "Temperature: "		at (0,indice+3).
	PRINT "Density: "			at (0,indice+4).
	PRINT "Pressure: "			at (0,indice+5).
	PRINT "Speed of Sound: "	at (0,indice+6).
	PRINT "Mach Number: "		at (0,indice+7).
	PRINT "Dynamic Pressure: "	at (0,indice+8).
	
	PRINT ROUND(tempVal,1)+" Cº   "			at (22,indice+3).
	PRINT ROUND(rhoVal,3)+" kg/m^3   "		at (22,indice+4).
	PRINT ROUND(pVal,3)+" (hPa)     "		at (22,indice+5).
	PRINT ROUND(soundVal,1)+" m/sec   "		at (22,indice+6).
	PRINT ROUND(machVal,2)+"   "			at (22,indice+7).
	PRINT ROUND(qVal,1)+" N/m^2    "		at (22,indice+8).

	// KSP-Values:
	// -----------
	PRINT "KSP-Pressure: "		at (0,indice+10).
	PRINT "KSP-Temperature: "	at (0,indice+11).
	PRINT "KSP-Dynamic Pressure: "	at (0,indice+12).
	PRINT ROUND(BODY:ATM:ALTITUDEPRESSURE(h),4)+" atm     "		at (22,indice+10).
	PRINT ROUND(BODY:ATM:ALTITUDETEMPERATURE(h),1)+" K     "	at (22,indice+11).
	PRINT ROUND(SHIP:Q,3)+"     "								at (22,indice+12).
	
	//Algorithm-2:	
	// -----------
	//PRINT "Pressure: "+( ROUND (Pressure(h)/100,2))+" (hPa)    " 	at (0,indice+12).
	//PRINT "Temperature: "+ROUND (Temperature(h)) + "    " 		at (0,indice+13).
}

LOG   "END: atm.c" to LOG_FILE.
LOG   "-----------------" to LOG_FILE.