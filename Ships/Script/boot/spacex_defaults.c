//Define Offline (defautl) Values: https://www.fcc.gov/media/radio/dms-decimal
declare global offline_LandingZone1 to latlng(28.612903098335448, -80.619702431884079).
declare global offline_LandingZone2 to latlng(28.612890963687462, -80.621613149083217).

declare global offline_LandingStarBase to latlng(25.9966111942016, -97.1497305858517).

//Define VESSELS on SAVED LOAD GAME: ""
declare global  LZ_1 to "LandingZone1". 				
declare global  LZ_2 to "LandingZone2". 				//R: MASTER
declare global  OCISLY to "OCISLY".
declare global  JRTI to "JRTI".
declare global  OCISLY_FAROUT to "OCISLY-FAROUT".

//Define Globals:
declare global mission_target to BODY.						// Planet Name - Sample: Kerbin, Earth
declare global LandingZone TO SHIP. //VESSEL(LZ_1).			// VESSEL NAME - Sample: LandingZone1, LandingZone2
declare global LandingTarget TO LandingZone:GEOPOSITION.	// Landing Target Geoposition