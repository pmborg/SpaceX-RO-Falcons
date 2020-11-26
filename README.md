# SpaceX-RO-Falcons
This KSP (**Kerbal Space Program**) addon, works ontop of [kks-spacex-pack mod](https://forum.kerbalspaceprogram.com/index.php?/topic/193933-1101-kks-spacex-pack-v65-01112020/) for KSP.<br>
This addon Introduce a Realistic Performance of SpaceX Falcon Family to KSP users.<br>
<br>
**Supported versions are:**
- KSP 1.9.1 (ISS - The International space station is only supported on this version)<br>
- KSP 1.10.1<br>

----
<H1>Regression Tests for KOS Code:</H2>

[ok] **F9 QMAX**<br>
[ok] **F9 ST-1 STAGE**<br>
[ok] **F9 ST-1 LANDING**<br>
[ok] **F9 ST-2 LEO ORBIT**<br>
<br>
[ok] **F9 + Crew Dragon 2 QMAX**<br>
[ok] **F9 + Crew Dragon 2 STAGE**<br>
[ok] **F9 + Crew Dragon 2 ST-1 LANDING**<br>
[ok] **F9 + Crew Dragon 2 LEO ORBIT**<br>
<br>
[ok] **FH ST1 QMAX**<br>
[ok] **FH ST1 STAGE**<br>
[ok] **FH ST1 Master ST-1 LANDING**<br>
[ok] **FH LEO Orbit**<br>
[ok] **FH Core ST-1 LANDING**<br>
<br>
[on development] FH GSO Orbit<br>
[checking] FH Slave ST-1 LANDING		Checking the impossible (at KSP) land of both ST-1 at same time...<br>

----
<h2>HOW TODO: Automatic Installation:</h2>

[1] First **do a Kerbal Space Program clean Install**, to avoid mod conflicts.

[2] **Copy all files from** https://github.com/pmborg/SpaceX-RO-Falcons/archive/main.zip **to [KSP ROOT]**<br>
(Sample: C:\SteamLibrary\steamapps\common\Kerbal Space Program)<br>

[3] then after the copy, Run the .BAT file: 
(**Run Script From**: [KSP ROOT]\SteamLibrary\steamapps\common\Kerbal Space Program)<br>
 **Pmborg-RealFalcons-v2-Install-For_KSP_1.9.1-All_Dependencies.bat**<br>
 --or--(depending of your case)<br>
 **Pmborg-RealFalcons-v2-Install-For_KSP_1.10.1-All_Dependencies.bat**<br>
 
![!](https://i.imgur.com/uNntmol.png)
 
**Note:**<br> 
After the instalation, during the KSP Loading, there will be some addon's that will rise some warnings about *outdated versions*, but that is "normal", they were not compiled for KSP 1.10.1 but they will work.

----
<h2>HOW TODO: Automatic Mod dependencies Update:</h2>

- RUN  **Pmborg-RealFalcons-v2-CheckForModUpdates.bat**<br>

----
<h2>Demo a Sample Mission, (load this saved game):</h2>

saves\InterStar\\**KSP1-9-BASE-DroneShip-LZ1-LZ2-ISS.sfs**<br>
(Include: LZ-1, LZ-2, OCISLY, JRTI, ISS and F9)<br>
--OR--<br>
saves\InterStar\\**KSP1-10-DEMO-BASE-DroneShip1-2-LZ1-2.sfs**<br>
(Include: LZ-1, LZ-2, OCISLY, JRTI and F9)<br>

Select: 0 3 [ENTER] at KOS Console Launch Menu.

If you want to follow Stage-1 landing, switch vessels imediattly after the stage separation.

----
<h2>HOW TO: Disable the KOS automatic Launch and Landing:</h2>

Rename directory:<br>

**[KSP ROOT]**/Ships/Script/**boot** TO **[KSP ROOT]**/Ships/Script/**nouse.boot**


----
<h2>YouTube video demos Playlist:</h2>

[![YouTube video Demos](blob:https://imgur.com/2e364972-9526-46f9-be95-8cb1b43dc7d2)](https://www.youtube.com/playlist?list=PLKyZSZ3Rbi6afgvRnMc2XUgyUA8dcYNkP)

**Crew Dragon Launch:**
![!](https://i.imgur.com/AVH8tmX.png)

**Falcon Heavy Launch:**
![!](https://i.imgur.com/xonhB3n.png)

**Stage-1 Landing on DroneShip (JRTI):**
![!](https://i.imgur.com/Qd0NqYw.png)

**Stage-1 Landing on LandingZone (LZ-2):**
![!](https://i.imgur.com/iUABkp8.png)

**ISS: For (KSP1.9.1 Only)**
![!](https://i.imgur.com/DZpBFGa.png)
