echo Please make sure that:
echo 
echo - ckan.exe GUI is not running.
echo - and KSP.exe is not running.
echo 

echo "Upgrading all Dependency Mods:"
ckan.exe upgrade --all
ckan.exe upgrade --headless KOS="1:1.2.1.0"
ckan.exe upgrade --headless RealFuels="rf-v12.9.1"

echo "RE-INSTALLING LATEST: Pmborg-RealFalcons"
set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
MKDIR %KSPTEMP%
powershell -command "& { iwr https://github.com/pmborg/SpaceX-RO-Falcons/archive/main.zip -OutFile main.zip }"
powershell.exe -NoP -NonI -Command "Expand-Archive 'main.zip' '%KSPTEMP%'"
xcopy /Y /S %KSPTEMP%\SpaceX-RO-Falcons-main .
