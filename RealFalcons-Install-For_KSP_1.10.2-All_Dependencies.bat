@echo off
REM 
REM First  - Copy all these files to "KSP MAIN DIRECTORY" (sample: C:\SteamLibrary\steamapps\common\Kerbal Space Program)
REM Second - Run the Script: Pmborg-RealFalcons-v2-Install-For_KSP_1.10.1-All_Dependencies
REM -------- This Script will download ckan and install all Dependencies for Pmborg-RealFalcons-v2 (For a clean instalation of KSP 1.10.1)
REM

CLS
TYPE HowToInstall.txt
echo.
echo "INSTALLING ON:"
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

	call RealFalcons-00-install-ckan
	
	REM echo "Get Latest CKAN Version:"
	REM powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile ckan.exe }"

	REM ckan.exe ksp default auto
	REM ckan.exe repo add Kopernicus_BE http://glacialsoftware.net/CKAN/Kopernicus_BE.tar.gz
	REM ckan.exe update
	REM ckan.exe cache reset
	REM ckan.exe compat add 1.10.0
	REM ckan.exe compat add 1.9.0
	REM ckan.exe compat add 1.8.1

	REM Install Special Kopernicus dev branch for KSP1.10.1:
	ckan.exe install --headless --allow-incompatible --no-recommends Kopernicus-BE

	REM General Base Stuff:
	ckan.exe install --headless --allow-incompatible --no-recommends KerbalXMod CraftManager KSP-AVC HangerExtenderExtended MoarFEConfigs DraggableNavball 
	REM VesselMoverContinued DraggableAltimeter 
	ckan.exe install --headless --allow-incompatible --no-recommends AntennaHelper BetterCrewAssignment KerbalAlarmClock Trajectories PatchManager 
	ckan.exe install --headless --allow-incompatible --no-recommends AmpYearPowerManager DMagicScienceAnimate
	REM EasyVesselSwitch SCANsat DockingCamKURS 
	ckan.exe install --headless --allow-incompatible --no-recommends CommunityTechTree 
	ckan.exe install --headless --allow-incompatible --no-recommends B9PartSwitch KSP-Recall 
	ckan.exe install --headless --allow-incompatible --no-recommends KerbalJointReinforcementContinued

	REM RealismOverhaul:
	echo "NOTE FOR NEXT Multi select option choose: RealismOverhaul"
	ckan.exe install --allow-incompatible --no-recommends RealFuels="rf-v12.9.1"
	ckan.exe install --headless --allow-incompatible --no-recommends RealHeat
	ckan.exe install --headless --allow-incompatible --no-recommends ROTanks ROSolar 

	REM RSS (Real Solar System):
	ckan.exe install --headless --allow-incompatible --no-recommends RSSTextures4096 RSSDateTimeFormatter KSCSwitcher RealSolarSystem

	REM SmokeScreen (already as dep)
	ckan.exe install --headless --allow-incompatible --no-recommends Scatterer Scatterer-config Scatterer-sunflare
	ckan.exe install --headless --allow-incompatible --no-recommends DynamicBatteryStorage ShipEffectsContinued EnvironmentalVisualEnhancements

	REM KIS-KAS:
	REM ckan.exe install --headless --allow-incompatible --no-recommends KIS KAS

	REM KOS:
	ckan.exe install --headless --allow-incompatible --no-recommends KOS

	REM DEPS:
	ckan.exe install --headless --allow-incompatible --no-recommends AnimatedDecouplers BackgroundResources FASA HeatControl KerbalEngineerRedux 
	REM MechJeb2 AlternateResourcePanel TACLS WorldStabilizer
	ckan.exe install --headless --allow-incompatible --no-recommends RetractableLiftingSurface SpaceXLaunchVehicles USITools SpaceXLegs
	ckan.exe install --headless --allow-incompatible --no-recommends BDAnimationModules DockingPortAlignmentIndicator REPOSoftTech-Agencies TextureReplacer

	REM Acelerometers needed to Falcons:
	ckan.exe install --headless --allow-incompatible --no-recommends TantaresSP

	pause

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

	REM Move Not Needed/Incompatible Stuff to "NONEED" directory:
	set NONEED=noneed~%RANDOM%.todelete
	mkdir %NONEED%
	move GameData\RealChute %NONEED%
	move GameData\FerramAerospaceResearch %NONEED%
	move GameData\AJE %NONEED%
	move GameData\EngineGroupController %NONEED%

	REM Add a filter to PMBORG RO ships, only:
	mkdir Ships_VAB
	move Ships\VAB\*.* Ships_VAB

	echo set kspver to 1.10. > Ships\Script\kspver.c.
	PAUSE
:No
	EXIT