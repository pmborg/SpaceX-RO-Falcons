@echo off

REM IMPORTANT:
REM After a clean install of "Kerbal Space Program" v1.11 or v1.12

REM STEPS:
REM First  - Copy this installer file to "KSP MAIN DIRECTORY" (sample: C:\SteamLibrary\steamapps\common\Kerbal Space Program)
REM Second - Run this Script: ULTRA-EVO-INSTALLER-v1.0-KSP1.11+.bat

echo "This script v1.0 can be used on KSP 1.11 or 1.12"

set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
MKDIR %KSPTEMP%

if not exist buildID64.txt (
	echo.
	echo "Installation error, the script is not located at KSP root directory"
	echo "SAMPLE: C:\SteamLibrary\steamapps\common\Kerbal Space Program"
	echo "-------------------------------------------------------------------"
	echo "Please check the videos on the wiki:"
	echo "https://github.com/pmborg/SpaceX-RO-Falcons/wiki/HOW-TO-INSTALL"
	goto :No 
	)
	

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

:No
PAUSE
EXIT