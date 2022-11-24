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

    REM DOWNLOAD Latest:
    REM echo "Get CKAN Latest Version"
    REM powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile ckan.exe }"

    REM Install Normal Kopernicus
    ckan.exe install --headless --allow-incompatible --no-recommends Kopernicus

    REM TO avoid Multiple Bugs:
	ckan.exe install --headless --allow-incompatible --no-recommends ZeroMiniAVC="1:1.1.0.2"
	
    REM General Base Stuff:
	ckan.exe install --headless --allow-incompatible --no-recommends CraftManager
    ckan.exe install --headless --allow-incompatible --no-recommends HangerExtenderExtended MoarFEConfigs DraggableNavball

    ckan.exe install --headless --allow-incompatible --no-recommends BetterCrewAssignment KerbalAlarmClock Trajectories PatchManager
	
    ckan.exe install --headless --allow-incompatible --no-recommends AmpYearPowerManager DMagicScienceAnimate

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
                                                                          
    ckan.exe install --headless --allow-incompatible --no-recommends MechJeb2 PersistentThrust
                
    mkdir Ships_VAB
    move Ships\VAB\*.* Ships_VAB
    ckan.exe install --headless --allow-incompatible --no-recommends RetractableLiftingSurface SpaceXLaunchVehicles USITools SpaceXLegs
    ckan.exe install --headless --allow-incompatible --no-recommends BDAnimationModules DockingPortAlignmentIndicator REPOSoftTech-Agencies TextureReplacer

    REM Acelerometers needed to Falcons:
    ckan.exe install --headless --allow-incompatible --no-recommends TantaresSP

    REM To show FPS (press F8 and drag it with mouse)
    ckan.exe install --headless --allow-incompatible --no-recommends ShowFPS

	ckan.exe install --headless --allow-incompatible --no-recommends BDAnimationModules DockingPortAlignmentIndicator TextureReplacer

    set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
    MKDIR %KSPTEMP%
    
	REM INSTALL: Needed for Crew Dragon & ISS (International SpaceStation) Dep-1:
    powershell -command "& { iwr https://spacedock.info/mod/2078/HabTech2/download/0.2.0 -OutFile HabTech2-0.2.0.zip }"
    powershell.exe -NoP -NonI -Command "Expand-Archive 'HabTech2-0.2.0.zip' '%KSPTEMP%'"
    REM mkdir GameData\HabTech2
    REM xcopy /S %KSPTEMP%\HabTech2_0.2.0\GameData\HabTech2 GameData\HabTech2
    mkdir GameData\Benjee10_sharedAssets
    xcopy /S %KSPTEMP%\HabTech2_0.2.0\GameData\Benjee10_sharedAssets GameData\Benjee10_sharedAssets
    move HabTech2-0.2.0.zip %KSPTEMP%

    REM INSTALL: Needed for Crew Dragon & ISS (International SpaceStation) Dep-2:
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
    REM move GameData\RSSDateTime %NONEED%
    move GameData\PatchManager %NONEED%
    move GameData\ShipEffectsContinued %NONEED%
    move GameData\TextureReplacer %NONEED%
    move GameData\EngineLightRelit %NONEED%

    REM Add a filter to PMBORG RO ships, only:
    mkdir Ships_VAB
    move /Y Ships\VAB\*.* Ships_VAB

    REM call OPTIONAL-Add_Katniss.s.Cape.Canaveral.bat
    echo set kspver to 1.12. > Ships\Script\kspver.c.
    echo.
    echo "Installation ended."
    echo "Please keep KOS at this version (1:1.2.1.0), and for future updates run scipt: RealFalcons-AUTO-Update.bat"
	echo "Please keep RealFuels at this version (rf-v12.9.1), and for future updates run scipt: RealFalcons-AUTO-Update.bat"
	echo "Please keep ZeroMiniAVC at this version (1:1.1.0.2), and for future updates run scipt: RealFalcons-AUTO-Update.bat"
    
    REM ADD StarShip deps:
    ckan.exe install --headless --allow-incompatible --no-recommends AT-Utils Waterfall TundraExploration
	
	REM Remove duplicated dll:
	REM move GameData\KXAPI\Plugins\KatLib.dll GameData\KXAPI\Plugins\KatLib.dll_
	
	REM CLEAN NOT NEED FILES (make it simple):
	REM move other craft files to a backup directory:
	move /Y Ships\VAB\*.* Ships_VAB
	mkdir Ships_SPH
    move /Y Ships\SPH\*.* Ships_SPH
	move /Y %NONEED% %KSPTEMP%
	move /Y .gitignore %KSPTEMP%
	REM move /Y RealFalcons-Install-* %KSPTEMP%
	
	REM delete other saved games other than KSP default ones and DEMO:
	pushd "saves" || exit /B 1
	for /D %%D in ("*") do (
		if /I not "%%~nxD"=="InterStar"  if /I not "%%~nxD"=="default" if /I not "%%~nxD"=="scenarios" if /I not "%%~nxD"=="training" rd /S /Q "%%~D"
	)
	for %%F in ("*") do (
		REM del "%%~F"
		move /Y "%%~F" %KSPTEMP%
	)
	
	REM NO NEED IN KSP1.12:
	move /Y InterStar\KSP1-9-BASE-DroneShip-LZ1-LZ2-ISS.loadmeta InterStar\KSP1-9-BASE-DroneShip-LZ1-LZ2-ISS.loadmeta_
	move /Y InterStar\KSP1-9-BASE-DroneShip-LZ1-LZ2-ISS.sfs InterStar\KSP1-9-BASE-DroneShip-LZ1-LZ2-ISS.sfs_
	popd
	
	REM NO NEED IN KSP1.12:
	move /Y Ships\VAB\NASA\Apollo11-4KSP1_11.craft Ships\VAB\NASA\Apollo11-4KSP1_11.craft_
	move /Y "Ships\VAB\ISS\PMB ISS HT2 & TANTARES KSP1_10.craft" "Ships\VAB\ISS\PMB ISS HT2 & TANTARES KSP1_10.craft_"
	move /Y "Ships\VAB\ISS\PMB ISS HT2 & TANTARES.craft" "Ships\VAB\ISS\PMB ISS HT2 & TANTARES.craft_"
	
	REM FIX1: Fix perfomance / bug on Kopernicus:
	copy /Y GameData\ModularFlightIntegrator\ModularFlightIntegrator.dll_ GameData\ModularFlightIntegrator\ModularFlightIntegrator.dll
	copy /Y GameData\Kopernicus\Config\Kopernicus_Config.cfg_ GameData\Kopernicus\Config\Kopernicus_Config.cfg
	
	echo "PRESS KEY: |ENTER| -> TO START KSP or |CTRL-C| to abort..."
    PAUSE
	call KSP_x64.exe
:No
    EXIT