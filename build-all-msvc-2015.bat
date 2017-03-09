@echo off
SET VALID=0
SET PLATFORMVALID=0
SET CHECKQTVERTOOL="bin/isqt580-debug.exe"
if [%1] EQU [Debug]  SET VALID=1
if [%1] EQU [Release]  SET VALID=1
if [%1] EQU [Release] SET CHECKQTVERTOOL="bin/isqt580-release.exe"

if [%2] EQU [Win32]  SET PLATFORMVALID=1
if [%2] EQU [x64]  SET PLATFORMVALID=1

if [%PLATFORMVALID%] EQU [0] (
	echo Build script for all libraries for Microsoft Visual C++ 2015. Just run it from Command line of Visual Studio  
    echo Syntax:
	echo build-all-msvc-2015.bat "[Debug|Release]" "[Win32|x64]"
	exit /B 1
)
if [%VALID%] EQU [0]  (
	echo Build script for all libraries for Microsoft Visual C++ 2015. Just run it from Command line of Visual Studio  
    echo Syntax:
	echo build-all-msvc-2015.bat "[Debug|Release]" "[Win32|x64]"
	exit /B 1
)
@echo on
devenv libsaddy-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tools/atlasgen/atlasgen-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv plugins/dukpp-03/dukpp-03-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tools/exporter/exporter-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv plugins/freetype/saddy-ft-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv plugins/irrklang/irrklang-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tools/ifaceed/ifaceed/ifaceed-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv examples/game/game-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv examples/multithreading/multithreading-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv examples/stress-physics/stress-physics-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/animations/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/db/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/dukpp-03/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/freetype/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/hfsm/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/input/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/irrklang/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/p2d/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/pipeline/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/resource/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/sad/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tests/layouts/alltests-2015.vcxproj /Build "%1|%2" /out lastsolutionbuild.log
devenv tools/isqt580/isqt580.vcxproj  /Build "%1|%2" /out lastsolutionbuild.log
%CHECKQTVERTOOL%
if errorlevel 1 (
	devenv plugins/qt-widget/qt-widget.vcxproj  /Build "%1|%2" /out lastsolutionbuild.log
	devenv examples/example-game-in-qt/example-game-in-qt.vcxproj  /Build "%1|%2" /out lastsolutionbuild.log
)