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
	echo Build script for all libraries for Microsoft Visual C++ 2017. Just run it from Command line of Visual Studio
    echo Syntax:
	echo build-all-msvc.bat "[Debug|Release]" "[Win32|x64]"
	exit /B 1
)
if [%VALID%] EQU [0]  (
	echo Build script for all libraries for Microsoft Visual C++ 2017. Just run it from Command line of Visual Studio
    echo Syntax:
	echo build-all-msvc.bat "[Debug|Release]" "[Win32|x64]"
	exit /B 1
)
@echo on
devenv libsaddy.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tools/atlasgen/atlasgen.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv plugins/dukpp-03/dukpp-03.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv plugins/freetype/saddy-ft.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tools/exporter/exporter.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv plugins/irrklang/irrklang.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv plugins/dukpp-03-irrklang/dukpp-03-irrklang.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tools/ifaceed/ifaceed/ifaceed.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv examples/game/game.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv examples/multithreading/multithreading.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv examples/stress-physics/stress-physics.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/animations/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/db/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/dukpp-03/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/freetype/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/hfsm/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/input/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/irrklang/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/dukpp-03-irrklang/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/p2d/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/pipeline/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/resource/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/sad/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tests/layouts/alltests.vcxproj /Build "%1|%2" /out lastsolutionbuild.log || goto :error
devenv tools/isqt580/isqt580.vcxproj  /Build "%1|%2" /out lastsolutionbuild.log || goto :error
%CHECKQTVERTOOL%
if errorlevel 1 (
  devenv plugins/qt-widget/qt-widget.vcxproj  /Build "%1|%2" /out lastsolutionbuild.log
  devenv examples/example-game-in-qt/example-game-in-qt.vcxproj  /Build "%1|%2" /out lastsolutionbuild.log
)
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
