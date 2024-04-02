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
msbuild libsaddy.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tools/atlasgen/atlasgen.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild plugins/dukpp-03/dukpp-03.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild plugins/freetype/saddy-ft.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tools/exporter/exporter.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild plugins/irrklang/irrklang.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild plugins/dukpp-03-irrklang/dukpp-03-irrklang.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild plugins/imgui/imgui.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild plugins/phy52d/phy52d.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
REM msbuild tools/ifaceed/ifaceed/ifaceed.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild examples/game/game.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild examples/multithreading/multithreading.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild examples/stress-physics/stress-physics.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild examples/imgui/imgui.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild examples/shaders/shaders.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/animations/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/db/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/dukpp-03/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/freetype/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/hfsm/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/input/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/irrklang/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/dukpp-03-irrklang/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/p2d/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/pipeline/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/resource/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/sad/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tests/layouts/alltests.vcxproj /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
msbuild tools/isqt580/isqt580.vcxproj  /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
%CHECKQTVERTOOL%
if errorlevel 1 (
  msbuild plugins/qt-widget/qt-widget.vcxproj  /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false || goto :error
  msbuild examples/example-game-in-qt/example-game-in-qt.vcxproj  /m /verbosity:minimal /p:Configuration="%1" /p:Platform="%2"  /p:BuildProjectReferences=false  || goto :error
)
goto :EOF

:error
type lastsolutionbuild.log
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
