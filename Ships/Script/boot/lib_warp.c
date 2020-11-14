// --------------------------------------------------------------------------------------------
// Filename: lib_warp.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//				Used warp on orbital mechanics
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

function WarpOutofKerbinSOI {
	PRINT "PhaseI: Warp Out of Kerbin SOI".
	
	until FALSE {
		
		//if body:name = "Kerbol" OR body:name = "Sun" {
		//	set warp to 7.
		//}
		if body:name = DEFAULT_KSC OR body:name = "Mun" OR body:name = "Minmus" OR body:name = "Moon" {
			set warp to 6.
		} else {
			if warp < 6 {
				set warp to 0.
				break.
			}
		}
			
		WAIT 1.
		PRINT "body: " + body:name at (0,11).
		PRINT "ship:Orbit:TRANSITION: "+ship:Orbit:TRANSITION+"     " at (0,10). //FINAL
	}.
		
	set warp to 0.
}

function WarptoSOIIntercept {
	PRINT "Phase V-Warp to SOI Intercept".
	until FALSE {
		if body:name = mission_target {
			//if phaseAngle = 999
			//	set warp to 5.
			//else
			//	set warp to 7.
			set warp to 6.
		}
		
		PRINT ship:Orbit:TRANSITION +"      " at (20,4).
		PRINT "body:name: "+body:name +"      " at (20,5).
		WAIT 1.
		
		if body:name = DEFAULT_KSC OR body:name = "Sun"
			break.

		//if ship:Orbit:TRANSITION = "FINAL"
		//	break.
	}
}