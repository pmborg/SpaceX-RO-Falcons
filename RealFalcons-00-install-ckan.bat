@echo off
if "%TEMP%" == "" (echo The TEMP variable must be set. ;goto :NO)
if not exist "%TEMP%" mkdir %TEMP%

cd /D %~dp0

:CHOICE
REM 1.8.1:  2010.10.25 (not sup.)
REM 1.9.1:  2020.02.21
REM 1.10.1: 2020.07.21
REM 1.11.2: 2021.03.03
REM 1.12:   2021.06.17
REM 1.12.1: 2021.06.24
REM 1.12.2: 2021.07.28

REM 9:
type buildID64.txt | find "2020.02"
if %errorlevel% == 0 ( 
	set c=9
	goto KSP
)
REM 10:
type buildID64.txt | find "2020.07"
if %errorlevel% == 0 ( 
	set c=10
	goto KSP
)
REM 11:
type buildID64.txt | find "2021.03"
if %errorlevel% == 0 ( 
	set c=11
	goto KSP
)
REM 12:
type buildID64.txt | find "2021.06"
if %errorlevel% == 0 ( 
	set c=12
	goto KSP
)
type buildID64.txt | find "2021.07"
if %errorlevel% == 0 ( 
	set c=12
	goto KSP
)

echo valid numbers are:
REM 
echo 9
echo 10
echo 11
echo 12
set /P c=Which KSP Version (1.?)
REM if /I "%c%" EQU "8" goto :KSP
if /I "%c%" EQU "9" goto :KSP
if /I "%c%" EQU "10" goto :KSP
if /I "%c%" EQU "11" goto :KSP
if /I "%c%" EQU "12" goto :KSP
goto :CHOICE

:KSP
if not exist "%TEMP%\ckan.exe" (
echo "Get CKAN-Version: v1.29.2"
powershell -command "& { iwr https://github.com/KSP-CKAN/CKAN/releases/download/v1.29.2/ckan.exe -OutFile %TEMP%\ckan.exe }"
)

%TEMP%\ckan.exe cache reset
%TEMP%\ckan.exe list

echo "Get CKAN-Version: Latest"
powershell -command "& { $latestRelease = (Invoke-WebRequest https://github.com/KSP-CKAN/CKAN/releases/latest -Headers @{'Accept'='application/json'}) ; $json = $latestRelease.Content | ConvertFrom-Json ; $LATESTVERSION = $json.tag_name ; iwr https://github.com/KSP-CKAN/CKAN/releases/download/$LATESTVERSION/ckan.exe -OutFile %TEMP%\ckan.exe }"

if /I "%c%" EQU "9" %TEMP%\ckan.exe compat add 1.9.0
if /I "%c%" EQU "9" %TEMP%\ckan.exe compat add 1.8.1

if /I "%c%" EQU "10" %TEMP%\ckan.exe repo add Kopernicus_BE http://glacialsoftware.net/CKAN/Kopernicus_BE.tar.gz
if /I "%c%" EQU "10" %TEMP%\ckan.exe compat add 1.10
if /I "%c%" EQU "10" %TEMP%\ckan.exe compat add 1.10.0
if /I "%c%" EQU "10" %TEMP%\ckan.exe compat add 1.9.0
if /I "%c%" EQU "10" %TEMP%\ckan.exe compat add 1.8.1

if /I "%c%" EQU "11" %TEMP%\ckan.exe compat add 1.10
if /I "%c%" EQU "11" %TEMP%\ckan.exe compat add 1.10.1
if /I "%c%" EQU "11" %TEMP%\ckan.exe compat add 1.10.0
if /I "%c%" EQU "11" %TEMP%\ckan.exe compat add 1.9.0
if /I "%c%" EQU "11" %TEMP%\ckan.exe compat add 1.8.1

if /I "%c%" EQU "12" %TEMP%\ckan.exe compat add 1.10
if /I "%c%" EQU "12" %TEMP%\ckan.exe compat add 1.11
if /I "%c%" EQU "12" %TEMP%\ckan.exe compat add 1.12

%TEMP%\ckan.exe update
%TEMP%\ckan.exe cache reset

echo "compat list:"
%TEMP%\ckan.exe compat list

goto :END
:NO
echo "Instalation Error"
:END

copy /Y %TEMP%\ckan.exe .