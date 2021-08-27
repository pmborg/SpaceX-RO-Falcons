@echo off

REM IMPORTANT:
REM After a clean install of "Kerbal Space Program" v1.11 or v1.12

REM STEPS:
REM First  - Copy this installer file to "KSP MAIN DIRECTORY" (sample: C:\SteamLibrary\steamapps\common\Kerbal Space Program)
REM Second - Run this Script: ULTRA-EVO-INSTALLER-v1.0-KSP1.11+.bat

echo "This script v1.0 can be used on KSP 1.11 or 1.12"

if not exist buildID64.txt (
	echo.
	echo "Installation error, the script is not located at KSP root directory"
	echo "SAMPLE: C:\SteamLibrary\steamapps\common\Kerbal Space Program"
	echo "-------------------------------------------------------------------"
	echo "Please check the videos on the wiki:"
	echo "https://github.com/pmborg/SpaceX-RO-Falcons/wiki/HOW-TO-INSTALL"
	goto :No 
	)
	
set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
echo "Using %KSPTEMP% for downloads"
MKDIR %KSPTEMP%
	
echo "Downloading CKAN Version: v1.29.2"
powershell -command "& { iwr https://github.com/KSP-CKAN/CKAN/releases/download/v1.29.2/ckan.exe -OutFile ckan.exe }"
echo "Setup CKAN for this directory:"
ckan.exe ksp add auto .

REM DOWNLOAD Latest:
echo "Downloading CKAN Latest Version"
powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile ckan.exe }"
ckan.exe update
ckan.exe cache reset

REM RSS (Real Solar System):
REM ------------------------
REM * Real Solar System Textures - 16384 x 8192 v18.3 (cached)
REM * RSS DateTime Formatter 1.6.1.0 (cached)
REM * Real Solar System v18.1.5 (cached)
REM * Module Manager 4.2.1 (cached)
REM * Kopernicus Planetary System Modifier 2:release-1.12.1-59 (cached)
REM * ModularFlightIntegrator 1.2.10.0 (cached)
ckan.exe install --headless --allow-incompatible --no-recommends RSSTextures16K RSSDateTimeFormatter RealSolarSystem

REM Scatterer:
REM ----------
REM * Scatterer Default Config 3:v0.0772 (cached)
REM * Scatterer Sunflare 3:v0.0772 (cached)
REM * scatterer 3:v0.0772 (cached)	
ckan.exe install --headless --allow-incompatible --no-recommends Scatterer-config Scatterer-sunflare Scatterer

REM RSSVE-HR
ckan.exe install --headless --allow-incompatible --no-recommends RSSVE-HR

REM EVE:
REM ----
REM * Environmental Visual Enhancements Redux 3:1.11.3.1 (cached)
ckan.exe install --headless --allow-incompatible --no-recommends EnvironmentalVisualEnhancements

REM EngineLightRelit:
REM -----------------
REM * Toolbar Controller 1:0.1.9.4 (cached)
REM * Engine Lighting Relit 1.6.3 (cached)
REM * ClickThrough Blocker 1:0.1.10.17 (cached)
REM * Zero MiniAVC 1:1.1.0.2 (cached)
ckan.exe install --headless --allow-incompatible ToolbarController EngineLightRelit

REM BetterLookingOceans:
REM --------------------
REM * Better Looking Oceans - High Resolution 1.3.0.3 (cached)
ckan.exe install --headless --allow-incompatible --no-recommends BetterLookingOceans-HighRes

REM TUFX:
REM ----
REM * TextureReplacer v4.5.1 (cached)
REM * TUFX 1.0.2.3 (cached)	
ckan.exe install --headless --allow-incompatible --no-recommends TextureReplacer TUFX

REM unBlur:
REM -------
REM  * unBlur v0.5.0 (cached)
ckan.exe install --headless --allow-incompatible --no-recommends unBlur

REM EVO REQ.:
REM * Community Terrain Texture Pack 1:1.0.4 (cached)
ckan.exe install --headless --allow-incompatible --no-recommends CommunityTerrainTexturePack

REM INSTALL: EVO
echo "Downloading: EVO"
if not exist "GameData\EVO" (
	if "%KSPTEMP%" == "" (echo The TEMP variable must be set. ;goto :No)
	if not exist "%KSPTEMP%" mkdir %KSPTEMP%
	powershell -command "& { iwr https://github.com/Its-Just-Luci/EVO/releases/download/Alpha-v0.2a/EVO-64k.Alpha.v0.2.zip -OutFile EVO.zip }"
	echo "Installing: EVO"
	powershell.exe -NoP -NonI -Command "Expand-Archive 'EVO.zip' '%KSPTEMP%'"
	mkdir GameData\EVO
	xcopy /S %KSPTEMP%\EVO GameData\EVO
	move EVO.zip %KSPTEMP%
	)

REM INSTALL: EVO-ULTRA SETTINGS FILES
echo "Downloading: EVO-ULTRA SETTINGS"
powershell -command "& { iwr https://www.dropbox.com/s/hjy9grnhn7eojb6/EVO-Ultra.zip?dl=1 -OutFile EVO-Ultra.zip }"
echo "Installing: EVO-ULTRA SETTINGS"
powershell.exe -NoP -NonI -Command "Expand-Archive 'EVO-Ultra.zip' GameData"
move EVO-Ultra.zip %KSPTEMP%

REM Setup EVO-ULTRA: Merge EVO integration with RSSVE-HI:
echo "Install: EVO-ULTRA SETTINGS"
copy /Y GameData\EVO-Ultra\Kopernicus_Config.cfg_ GameData\Kopernicus\Config\Kopernicus_Config.cfg
copy /Y GameData\EVO-Ultra\EVO-Earth_Atmosphere.cfg_ GameData\EVO\Configs\Scatterer_Configs\Earth\Earth_Atmosphere.cfg
copy /Y GameData\EVO-Ultra\RSSVE-Earth_Atmosphere.cfg_ GameData\RSSVE\Configs\Scatterer_Configs\Earth\Earth_Atmosphere.cfg

type buildID64.txt | find "2021.06"
if %errorlevel% == 0 ( 
	echo "This instalation is for KSP 1.12.2"
	copy /Y GameData\EVO-Ultra\settings1.12.cfg_ settings.cfg
)
type buildID64.txt | find "2021.03"
if %errorlevel% == 0 ( 
	echo "This instalation is for KSP 1.11.2"
	copy /Y GameData\EVO-Ultra\settings1.11.cfg_ settings.cfg
)

move /Y GameData\RSSVE\Configs\Scatterer_Configs\RSSVE_Scatterer_Planetlist_Config.cfg GameData\RSSVE\Configs\Scatterer_Configs\RSSVE_Scatterer_Planetlist_Config.cfg_
move /Y GameData\RSSVE\Configs\Scatterer_Configs\RSSVE_Scatterer_Settings_Config.cfg GameData\RSSVE\Configs\Scatterer_Configs\RSSVE_Scatterer_Settings_Config.cfg_
move /Y GameData\RSSVE\Configs\Scatterer_Configs\RSSVE_Scatterer_Sunflare_Config.cfg GameData\RSSVE\Configs\Scatterer_Configs\RSSVE_Scatterer_Sunflare_Config.cfg_
move /Y GameData\RSSVE\Textures\DetailTextures %KSPTEMP%
move /Y GameData\RSSVE\Textures\MainTextures %KSPTEMP%
move /Y GameData\RSSVE\Textures\ScatterTextures %KSPTEMP%
move /Y GameData\EVO-Ultra %KSPTEMP%

REM ShowFPS:
REM --------
REM CORRECT:* Show FPS 1:0.2.4 (cached)
REM WRONG:  * Show FPS 1:0.3.0 (cached)
REM ShowFPS: (by default ShowFPS will install on KSP 1.11 the version targeted to 1.12, force/allow to use 1.11, to avoid that error)
ckan.exe compat add 1.11
ckan.exe install --headless --allow-incompatible --no-recommends ShowFPS="1:0.2.4"

:No
PAUSE
EXIT