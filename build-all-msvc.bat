@echo off
SET VALID=0
if [%1] EQU [Debug]  SET VALID=1
if [%1] EQU [Release]  SET VALID=1
if [%VALID%] EQU [0]  (
	echo Build script for all libraries for Microsoft Visual C++. Just run it from Command line of Visual Studio  
    echo Syntax:
	echo build-all-msvc.bat "[Debug|Release]"
	exit /B 1
)
@echo on
devenv.exe libsaddy.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe plugins/atlasgenc/atlasgen.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe plugins/duktape/duktape.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe plugins/exporter/exporter.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe plugins/freetype/saddy-ft.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe plugins/irrklang/irrklang.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe plugins/ifaceed/ifaceed/ifaceed.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe examples/game/game.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe examples/multithreading/multithreading.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe examples/stress-physics/stress-physics.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/db/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/duktape/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/freetype/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/hfsm/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/input/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/irrklang/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/p2d/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/pipeline/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/resource/alltests.vcxproj /Build %1 /out lastsolutionbuild.log
devenv.exe tests/sad/alltests.vcxproj /Build %1 /out lastsolutionbuild.log