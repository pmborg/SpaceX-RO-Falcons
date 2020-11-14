// --------------------------------------------------------------------------------------------
// Filename: lib_resources.c
// --------------------------------------------------------------------------------------------
// KOS Scripts for KSP to be used on SpaceX_Launch_Vehicles Mod with (Pmborg RO Version) 
// --------------------------------------------------------------------------------------------
// Code by : Pmborg - https://forum.kerbalspaceprogram.com/index.php?/profile/198861-pmborg/
// Beta load from: 	- https://www.dropbox.com/sh/jd1oh6d806iyat1/AABa7aXbiYDfv8G-aQ4MyR-ta?dl=0
// Latest Download: - https://github.com/pmborg/SpaceX-RO-Falcons
// Purpose: 
//              This is code is used to refuel a ship equiped with mining prospect material
//              to refuel the ship automatically.
// 14/Nov/2020
// --------------------------------------------------------------------------------------------

function ship_resource 
{
	PARAMETER resName.
	FOR res in SHIP:RESOURCES {
		IF res:NAME = resName {
			RETURN res.
		}
	}
}

function traversePartsForResources 
{
parameter part.

list parts in ps.
set i to ps:iterator.
	until not i:next {
	set p to i:value.
	if p:resources:length > 0 {

		set j to p:resources:iterator.
		until not j:next {
			set r to j:value.
			if r:enabled {
				set s to "ok".
			} else {
				set s to "disabled".
			}
			if r:name:find(part) > 0 {
				set text to r:name + ", (" + round(r:amount) + "/" + round(r:capacity) + ") " + round((r:amount / r:capacity)*100)+" %".
				PRINT text.
				LOG text to resources.txt.
			} else
				LOG r:name + ", (" + round(r:amount) + "/" + round(r:capacity) + ") " to resources.txt.
		}
	}
	}
}
