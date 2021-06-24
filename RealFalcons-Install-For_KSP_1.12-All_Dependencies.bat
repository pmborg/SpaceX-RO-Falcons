@echo off
REM 
REM First  - Copy all these files to "KSP MAIN DIRECTORY" (sample: C:\SteamLibrary\steamapps\common\Kerbal Space Program)
REM Second - Run the Script: Pmborg-RealFalcons-v2-Install-For_KSP_1.10.1-All_Dependencies
REM -------- This Script will download ckan and install all Dependencies for Pmborg-RealFalcons-v2 (For a clean instalation of KSP 1.10.1)
REM

CLS
TYPE HowToInstall.txt
echo.
cd

if not exist buildID64.txt (
	echo.
	echo "Installation error, the script is not located at KSP root directory"
	echo "-------------------------------------------------------------------"
	echo "Please check the videos on the wiki:"
	echo "https://github.com/pmborg/SpaceX-RO-Falcons/wiki/HOW-TO-INSTALL"
	pause
	goto :No 
	)

:choice
	set /P c=Do you wish to proceed with the install ? (Y/N)
	if /I "%c%" EQU "Y" goto :Yes
	if /I "%c%" EQU "y" goto :Yes
	if /I "%c%" EQU "N" goto :No
	if /I "%c%" EQU "n" goto :No
	goto :choice

:Yes
	if "%TEMP%" == "" (echo The TEMP variable must be set. ;goto :No)
	if not exist "%TEMP%" mkdir %TEMP%

	echo "Get CKAN Version: v1.29.2"
	powershell -command "& { iwr https://github.com/KSP-CKAN/CKAN/releases/download/v1.29.2/ckan.exe -OutFile ckan.exe }"

	ckan.exe ksp default auto
	REM ckan.exe repo add Kopernicus_BE http://glacialsoftware.net/CKAN/Kopernicus_BE.tar.gz
	ckan.exe update
	ckan.exe cache reset
	ckan.exe compat add 1.12
	ckan.exe compat add 1.11
	ckan.exe compat add 1.10
	ckan.exe compat add 1.10.1
	ckan.exe compat add 1.10.0
	ckan.exe compat add 1.9
	ckan.exe compat add 1.9.0
	ckan.exe compat add 1.8.1
	ckan.exe compat 1.11.2.3077
	ckan.exe compat 1.11.1.3066
	ckan.exe compat 1.11.0.3045
	ckan.exe compat 1.10.1.2939
	ckan.exe compat 1.10.0.2917
	ckan.exe compat 1.9.1.2788

	REM DOWNLOAD Latest:
	echo "Get CKAN Latest Version"
	powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile ckan.exe }"

	REM Install Normal Kopernicus
	ckan.exe install --headless --allow-incompatible --no-recommends Kopernicus

	REM General Base Stuff:
	ckan.exe install --headless --allow-incompatible --no-recommends CraftManager KerbalXMod HangerExtenderExtended MoarFEConfigs DraggableNavball

	ckan.exe install --headless --allow-incompatible --no-recommends BetterCrewAssignment KerbalAlarmClock PatchManager 
	ckan.exe install --headless --allow-incompatible --no-recommends Trajectories
	
	ckan.exe install --headless --allow-incompatible --no-recommends AmpYearPowerManager DMagicScienceAnimate 

	ckan.exe install --headless --allow-incompatible --no-recommends CommunityTechTree 
	ckan.exe install --headless --allow-incompatible --no-recommends B9PartSwitch KSP-Recall 
	ckan.exe install --headless --allow-incompatible --no-recommends KerbalJointReinforcementContinued

	REM RealismOverhaul:
	echo "NOTE FOR NEXT Multi select option choose: RealismOverhaul"
	ckan.exe install --allow-incompatible --no-recommends RealFuels
	ckan.exe install --headless --allow-incompatible --no-recommends RealHeat
	ckan.exe install --headless --allow-incompatible --no-recommends ROTanks ROSolar 

	REM RSS (Real Solar System):
	REM ckan.exe install --headless --allow-incompatible --no-recommends RSSDateTimeFormatter
	ckan.exe install --headless --allow-incompatible --no-recommends RSSTextures4096 KSCSwitcher RealSolarSystem
	

	REM SmokeScreen (already as dep)
	ckan.exe install --headless --allow-incompatible --no-recommends Scatterer Scatterer-config Scatterer-sunflare
	ckan.exe install --headless --allow-incompatible --no-recommends DynamicBatteryStorage ShipEffectsContinued EnvironmentalVisualEnhancements

	REM not used:
	REM KSP-AVC
	REM VesselMoverContinued DraggableAltimeter AntennaHelper
	REM EasyVesselSwitch SCANsat DockingCamKURS 
	REM ckan.exe install --headless --allow-incompatible --no-recommends KIS KAS
	REM AlternateResourcePanel TACLS WorldStabilizer
	
	REM KOS: (Use this stable version for now, once the new ones are generating several exceptions:)
	ckan.exe install --headless --allow-incompatible --no-recommends KOS="1:1.2.1.0"

	REM DEPS:
	ckan.exe install --headless --allow-incompatible --no-recommends AnimatedDecouplers BackgroundResources FASA HeatControl KerbalEngineerRedux 
	ckan.exe install --headless --allow-incompatible --no-recommends MechJeb2
	ckan.exe install --headless --allow-incompatible --no-recommends RetractableLiftingSurface SpaceXLaunchVehicles USITools SpaceXLegs
	ckan.exe install --headless --allow-incompatible --no-recommends BDAnimationModules DockingPortAlignmentIndicator REPOSoftTech-Agencies TextureReplacer

	REM Acelerometers needed to Falcons:
	ckan.exe install --headless --allow-incompatible --no-recommends TantaresSP

	REM ADD StarShip
	ckan.exe install --headless --allow-incompatible --no-recommends AT-Utils Waterfall TundraExploration

	REM To show FPS (press F8 and drag it with mouse)
	ckan.exe install --headless --allow-incompatible --no-recommends ShowFPS

	REM INSTALL: ISS (International SpaceStation) Dep-1:
	set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
	MKDIR %KSPTEMP%
	
	powershell -command "& { iwr https://spacedock.info/mod/2078/HabTech2/download/0.2.0 -OutFile HabTech2-0.2.0.zip }"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'HabTech2-0.2.0.zip' '%KSPTEMP%'"
	mkdir GameData\HabTech2
	xcopy /S %KSPTEMP%\HabTech2_0.2.0\GameData\HabTech2 GameData\HabTech2
	mkdir GameData\Benjee10_sharedAssets
	xcopy /S %KSPTEMP%\HabTech2_0.2.0\GameData\Benjee10_sharedAssets GameData\Benjee10_sharedAssets
	move HabTech2-0.2.0.zip %KSPTEMP%

	REM INSTALL: ISS (International SpaceStation) Dep-2:
	powershell -command "& { iwr https://archive.org/download/NewTantares-v18.0/C0E93925-NewTantares-v18.0.zip -OutFile NewTantares-v18.0.zip }"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'NewTantares-v18.0.zip' '%KSPTEMP%'"
	mkdir GameData\Tantares
	xcopy /S %KSPTEMP%\GameData\Tantares GameData\Tantares
	move NewTantares-v18.0.zip %KSPTEMP%
	
	REM INSTALL: SpaceX Towers
	powershell -command "& { iwr https://spacedock.info/mod/626/SpaceX%20Launch%20Towers/download/1.7.3 -OutFile SpaceX_Launch_Towers-1.7.3.zip }"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'SpaceX_Launch_Towers-1.7.3.zip' '%KSPTEMP%'"
	mkdir GameData\Science818
	xcopy /S %KSPTEMP%\GameData\Science818 GameData\Science818
	move SpaceX_Launch_Towers-1.7.3.zip %KSPTEMP%

	REM INSTALL: BETA Kopernicus for 1.12:
	powershell -command "& { iwr https://github.com/Kopernicus/Kopernicus/releases/download/release-46/Kopernicus-1.12.0-46.zip -OutFile Kopernicus.zip }"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'Kopernicus.zip' '%KSPTEMP%'"
	mkdir GameData\Kopernicus
	mkdir GameData\ModularFlightIntegrator
	xcopy /S /Y %KSPTEMP%\GameData\Kopernicus GameData\Kopernicus
	xcopy /S /Y %KSPTEMP%\GameData\ModularFlightIntegrator GameData\ModularFlightIntegrator
	move Kopernicus.zip %KSPTEMP%

	REM ...OR... INSTALL: EVO ULTRA:
	powershell -command "& { iwr https://github.com/Its-Just-Luci/EVO/releases/download/Alpha-v0.2a/EVO-64k.Alpha.v0.2.zip -OutFile EVO.zip }"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'EVO.zip' '%KSPTEMP%'"
	mkdir GameData\EVO
	xcopy /S %KSPTEMP%\EVO GameData\EVO
	move EVO.zip %KSPTEMP%

	REM Move Not Needed/Incompatible Stuff to "NONEED" directory:
	set NONEED=noneed~%RANDOM%.todelete
	mkdir %NONEED%
	move GameData\RealChute %NONEED%
	move GameData\FerramAerospaceResearch %NONEED%
	move GameData\AJE %NONEED%
	move GameData\EngineGroupController %NONEED%
	move GameData\EngineGroupController %NONEED%
	REM move GameData\KXAPI %NONEED%

	echo set kspver to 1.12. > Ships\Script\kspver.c.

	REM Add a filter to PMBORG RO ships, only:
	mkdir Ships\VAB.all
	move Ships\VAB\*.* Ships\SPH
	move Ships\VAB.all %NONEED%

    REM Using a beta version recompiled for KSP 1.12
	copy /Y GameData\Trajectories\Plugin\Trajectories.dll_beta GameData\Trajectories\Plugin\Trajectories.dll
	
	echo.
	echo "Installation ended."
	echo "Please keep KOS at this version (1:1.2.1.0), and for future updates run scipt: RealFalcons-AUTO-Update.bat"
	PAUSE
:No
	EXIT