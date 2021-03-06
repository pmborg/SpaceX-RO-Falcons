# WHAT WILL BE ON RELEASE 1 ?
What is on development [Not yet Available]<br>
-- [FH+LEM] On the Moon?<br>
-- [FH] Landing Both FH ST-1 Boosters at the same time on LZ-1 and LZ-2. **(Already work! tunning it...)**<br>


**Testing all Moon Mission code with SaturnV, (before use FH+LEM)**
![!](https://i.imgur.com/OXC7pUn.png)


**Note:** The SaturnV after fly certification will be also released.<br>


**Testing all Crew Dragon 2 Mission:**
![!](https://i.imgur.com/1L7eKOC.png)
----

# SpaceX-RO-Falcons
This KSP (**Kerbal Space Program**) addon, works ontop of [kks-spacex-pack mod](https://forum.kerbalspaceprogram.com/index.php?/topic/193933-1101-kks-spacex-pack-v65-01112020/) for KSP.<br>
The **SpaceX-RO-Falcons** Mod Introduce: 
* A realistic Performance of SpaceX Falcon family to KSP users.<br>
* Expand the physics of KSP to allow multiple ships flying at the same time, but with high performance.<br>
* All parts to create all the Falcon rocket family are available.
* LEO and GSO Payloads are allowed.
* Land or Sea landings is a *User Choise* ([KSP-ROOT-DIR]/Ships/Script/boot/**USER-CONFIG.KS**).
* Faring separation.
* ST-2 Deorbit.
* In resume all types of missions are coded in KOS.
* **Supported crafts:**<br>
───ISS<br>
│       PMB ISS HT2 & TANTARES KSP1_10.craft<br>
│       PMB ISS HT2 & TANTARES.craft<br>
│<br>
├───NASA<br>
│       LEM.craft (not included)<br>
│       PMBT-SpaceX Falcon Heavy v1_2 Block-5 LEM.craft (not included)<br>
│<br>
└───SpaceX<br>
&nbsp;&nbsp;&nbsp;   ├───Crew Dragon 2<br>
&nbsp;&nbsp;&nbsp;   │       PMB-SpaceX Crew Dragon 2.craft<br>
&nbsp;&nbsp;&nbsp;   │<br>
&nbsp;&nbsp;&nbsp;   ├───Falcon 1 (2006-2010)<br>
&nbsp;&nbsp;&nbsp;   │       PMBT-SpaceX Falcon 1 (Merlin 1A).craft<br>
&nbsp;&nbsp;&nbsp;   │       PMBT-SpaceX Falcon 1 (Merlin 1C).craft<br>
&nbsp;&nbsp;&nbsp;   │<br>
&nbsp;&nbsp;&nbsp;   ├───Falcon 9 (2010-2018)<br>
&nbsp;&nbsp;&nbsp;   │       PMBT-SpaceX Falcon 9 v1_0.craft<br>
&nbsp;&nbsp;&nbsp;   │       PMBT-SpaceX Falcon 9 v1_1 Block-2.craft<br>
&nbsp;&nbsp;&nbsp;   │       PMBT-SpaceX Falcon 9 v1_2 Block-3.craft<br>
&nbsp;&nbsp;&nbsp;   │       PMBT-SpaceX Falcon 9 v1_2 Block-4.craft<br>
&nbsp;&nbsp;&nbsp;   │<br>
&nbsp;&nbsp;&nbsp;   └───Falcon 9&FH v1.2 Block-5<br>
&nbsp;&nbsp;&nbsp;   &nbsp;&nbsp;&nbsp;   PMBT-SpaceX Falcon 9 v1_2 Block-5 Crew Dragon 2.craft<br>
&nbsp;&nbsp;&nbsp;   &nbsp;&nbsp;&nbsp;   PMBT-SpaceX Falcon 9 v1_2 Block-5.craft<br>
&nbsp;&nbsp;&nbsp;   &nbsp;&nbsp;&nbsp;   PMBT-SpaceX Falcon Heavy v1_2 Block-5.craft<br>

**Supported versions are:**
* **KSP 1.9.1** (include original ISS)<br>
* **KSP 1.10.1** ([NEW] now there is a new **ISS for KSP 1.10.1 & 1.11**)<br>
* **KSP 1.11.0** ([NEW] There is a new install bat file, to install this version)<br>

----
**CURRENT FLEET:**
![!](https://i.imgur.com/w9gUgOb.png)

----
<H1>Regression Tests for KOS Code:</H2>

[ok] **F1-M1C QMAX**<br>
[ok] **F1-M1C ST-1 STAGE**<br>
[ok] **F1-M1C ST-2 LEO ORBIT**<br>
<br>
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
 
![!](https://i.imgur.com/XtxincO.png)

You can also check the videos about HOW TO INSTALL in Wiki page:

**WIKI-HOW TO INSTALL**: (https://github.com/pmborg/SpaceX-RO-Falcons/wiki/HOW-TO-INSTALL)
 
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


# Falcon 9 Mission Video:
[![Falcon 9 Mission - How it works?](https://i.imgur.com/l2MbG4p.png)](https://www.youtube.com/watch?v=ktnvzGVYlrM)

----
**Earth with Real Solar System Mod: (NOW also on KSP 1.11)**
![!](https://i.imgur.com/7XuzhOa.png)

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
