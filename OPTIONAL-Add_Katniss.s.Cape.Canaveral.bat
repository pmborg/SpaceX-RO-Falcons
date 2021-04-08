REM 
REM This Allow to add: Katniss.s.Cape.Canaveral
REM 

set KSPTEMP=%TEMP%\ksp~%RANDOM%.tmp
MKDIR %KSPTEMP%

REM INSTALL: Katniss.s.Cape.Canaveral (this will be an option)
powershell -command "& { iwr https://github.com/Katniss218/KatnisssCapeCanaveral/releases/download/1.2.2/Katniss.s.Cape.Canaveral.v1.2.2.zip -OutFile Katniss.s.Cape.Canaveral.v1.2.2.zip }"
powershell.exe -NoP -NonI -Command "Expand-Archive 'Katniss.s.Cape.Canaveral.v1.2.2.zip' '%KSPTEMP%'"
mkdir GameData\KatnisssCapeCanaveral
mkdir GameData\RSS-Textures
mkdir GameData\RSS-Tweaks
xcopy /S /Y %KSPTEMP%\GameData\KatnisssCapeCanaveral GameData\KatnisssCapeCanaveral
xcopy /S /Y %KSPTEMP%\GameData\RSS-Textures GameData\RSS-Textures
xcopy /S /Y %KSPTEMP%\GameData\RSS-Tweaks GameData\RSS-Tweaks
move Katniss.s.Cape.Canaveral.v1.2.2.zip %KSPTEMP%

cd saves\InterStar
ren *.sfs *.sfs.default
ren *.loadmeta *.loadmeta.default
ren *.option *.
