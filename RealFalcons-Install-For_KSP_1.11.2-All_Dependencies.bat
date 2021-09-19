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

	REM echo "Get CKAN Version: v1.29.2"
	REM powershell -command "& { iwr https://github.com/KSP-CKAN/CKAN/releases/download/v1.29.2/ckan.exe -OutFile ckan.exe }"

	REM ckan.exe ksp default auto
	REM ckan.exe repo add Kopernicus_BE http://glacialsoftware.net/CKAN/Kopernicus_BE.tar.gz
	REM ckan.exe update
	REM ckan.exe cache reset
	REM ckan.exe compat add 1.10.1
	REM ckan.exe compat add 1.10.0
	REM ckan.exe compat add 1.9.0
	REM ckan.exe compat add 1.8.1

	REM DOWNLOAD Latest:
	REM echo "Get CKAN Latest Version"
	REM powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile ckan.exe }"

	REM Install Special Kopernicus dev branch for KSP1.10.1 and KSP1.11.x:
	ckan.exe install --headless --allow-incompatible --no-recommends Kopernicus

	REM General Base Stuff:
	ckan.exe install --headless --allow-incompatible --no-recommends CraftManager HangerExtenderExtended MoarFEConfigs DraggableNavball
	REM KerbalXMod

	ckan.exe install --headless --allow-incompatible --no-recommends BetterCrewAssignment KerbalAlarmClock Trajectories PatchManager 
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
	ckan.exe install --headless --allow-incompatible --no-recommends RSSTextures4096 RSSDateTimeFormatter KSCSwitcher RealSolarSystem

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
	REM BUG FIXED by pmborg: (faster version)
	copy /Y GameData\KerbalEngineer\KerbalEngineer.dll_ GameData\KerbalEngineer\KerbalEngineer.dll
	
	ckan.exe install --headless --allow-incompatible --no-recommends MechJeb2
	ckan.exe install --headless --allow-incompatible --no-recommends RetractableLiftingSurface SpaceXLaunchVehicles USITools SpaceXLegs
	ckan.exe install --headless --allow-incompatible --no-recommends BDAnimationModules DockingPortAlignmentIndicator REPOSoftTech-Agencies TextureReplacer

	REM Acelerometers needed to Falcons:
	ckan.exe install --headless --allow-incompatible --no-recommends TantaresSP

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

	REM INSTALL: KopernicusBE for 1.11.2:
	powershell -command "& { iwr https://github.com/R-T-B/Kopernicus/releases/download/UBE-release-81/KopernicusBE_19+_Release81.zip -OutFile KopernicusBE.zip }"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'KopernicusBE.zip' '%KSPTEMP%'"
	mkdir GameData\Kopernicus
	mkdir GameData\ModularFlightIntegrator
	xcopy /S /Y %KSPTEMP%\GameData\Kopernicus GameData\Kopernicus
	xcopy /S /Y %KSPTEMP%\GameData\ModularFlightIntegrator GameData\ModularFlightIntegrator
	move KopernicusBE.zip %KSPTEMP%

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
	move GameData\RSSDateTime %NONEED%
	move GameData\PatchManager %NONEED%
	move GameData\ShipEffectsContinued %NONEED%
	move GameData\TextureReplacer %NONEED%
	move GameData\EngineLightRelit %NONEED%
	
	REM call OPTIONAL-Add_Katniss.s.Cape.Canaveral.bat

	echo set kspver to 1.11. > Ships\Script\kspver.c.

	REM Add a filter to PMBORG RO ships, only:
	mkdir Ships_VAB
	move Ships\VAB\*.* Ships_VAB

	echo.
	echo "Installation ended."
	echo "Please keep KOS at this version (1:1.2.1.0), and for future updates run scipt: RealFalcons-AUTO-Update.bat"
	
	call ULTRA-EVO-INSTALLER-v1.0-KSP1.11+.bat
	
	REM ADD StarShip
	ckan.exe install --headless --allow-incompatible --no-recommends AT-Utils Waterfall TundraExploration
	
	PAUSE
:No
	EXIT