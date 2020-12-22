@echo off
echo "First install KSP 1.11 from scratch/clean instalation..."

if not exist buildID64.txt (
	echo.
	echo "Installation error, the script is not located at KSP root directory"
	echo "[Sample] C:\SteamLibrary\steamapps\common\Kerbal Space Program"
	echo.
	echo "Please check the videos on: https://www.youtube.com/channel/UCF-1s3VtQu9DiFJPgzRgvaA"
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

echo "Get Latest CKAN Version:"
powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile ckan.exe }"

echo "Setup CKAN for KSP 1.11:"
ckan.exe ksp default auto
ckan.exe update
ckan.exe cache reset
ckan.exe compat add 1.11
ckan.exe compat add 1.10.1
ckan.exe compat add 1.10.0
ckan.exe compat add 1.9.0

echo "Install (Parallax+EVE):"
ckan.exe install --headless --allow-incompatible --no-recommends Parallax-StockTextures Parallax 
ckan.exe install --headless --allow-incompatible --no-recommends Scatterer-config Scatterer-sunflare Scatterer
ckan.exe install --headless --allow-incompatible --no-recommends EnvironmentalVisualEnhancements EnvironmentalVisualEnhancements-HR

echo "INSTALL: KopernicusBE_111"
set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
MKDIR %KSPTEMP%
powershell -command "& { iwr https://github.com/R-T-B/Kopernicus/releases/download/UBE-release-56/KopernicusBE_111_Release57.zip -OutFile KopernicusBE_111_Release57.zip }"
powershell.exe -NoP -NonI -Command "Expand-Archive 'KopernicusBE_111_Release57.zip' '%KSPTEMP%'"
mkdir GameData\Kopernicus
mkdir GameData\ModularFlightIntegrator
xcopy /S /Y %KSPTEMP%\GameData\Kopernicus GameData\Kopernicus
xcopy /S /Y %KSPTEMP%\GameData\ModularFlightIntegrator GameData\ModularFlightIntegrator
move KopernicusBE_111_Release57.zip %KSPTEMP%

:No
	EXIT