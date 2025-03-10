@echo off
SET VALID=0
SET BatchToRun=
SET QtConfig=
SET CHECKQTVERTOOL=
if [%1] EQU [Debug]  (
	SET VALID=1
	SET BatchToRun=debug.bat
	SET QtConfig=debug
	SET CHECKQTVERTOOL="bin/isqt580-debug.exe"
)
if [%1] EQU [Release]  (
	SET VALID=1
	SET BatchToRun=release.bat
	SET QtConfig=release
	SET CHECKQTVERTOOL="bin/isqt580-release.exe"
)
if [%VALID%] EQU [0]  (
	echo Build script for all libraries for MinGW. Just run it from Command line of Qt
    echo Syntax:
	echo build-all-mingw.bat "[Debug|Release]"
	exit /B 1
)
@echo on
call %BatchToRun%
cd tools/atlasgen
qmake CONFIG+=%QtConfig% atlasgen.pro || goto :error
mingw32-make || goto :error
cd ../..
cd tools/exporter
qmake CONFIG+=%QtConfig% exporter.pro || goto :error
mingw32-make || goto :error
cd ../..
cd plugins/freetype
call %BatchToRun% || goto :error
cd ../..
REM cd tools/ifaceed
REM qmake CONFIG+=%QtConfig% ifaceed.pro
REM mingw32-make
REM cd ../..
cd plugins/dukpp-03
call %BatchToRun% || goto :error
cd ../..
cd plugins/irrklang
call %BatchToRun% || goto :error
cd ../..
cd plugins/imgui
call %BatchToRun% || goto :error
cd ../..
cd plugins/phy52d
call %BatchToRun% || goto :error
cd ../..
cd plugins/dukpp-03-irrklang
call %BatchToRun%
cd ../..
cd examples/game
call %BatchToRun% || goto :error
cd ../..
cd examples/multithreading
call %BatchToRun% || goto :error
cd ../..
cd examples/stress-physics
call %BatchToRun% || goto :error
cd ../..
cd examples/imgui
call %BatchToRun% || goto :error
cd ../..
cd examples/shaders
call %BatchToRun% || goto :error
cd ../..
cd tests/animations
call %BatchToRun% || goto :error
cd ../..
cd tests/db
call %BatchToRun% || goto :error
cd ../..
cd tests/dukpp-03
call %BatchToRun% || goto :error
cd ../..
cd tests/freetype
call %BatchToRun% || goto :error
cd ../..
cd tests/hfsm
call %BatchToRun% || goto :error
cd ../..
cd tests/input
call %BatchToRun% || goto :error
cd ../..
cd tests/irrklang
call %BatchToRun% || goto :error
cd ../..
cd tests/dukpp-03-irrklang
call %BatchToRun% || goto :error
cd ../..
cd tests/p2d
call %BatchToRun% || goto :error
cd ../..
cd tests/pipeline
call %BatchToRun% || goto :error
cd ../..
cd tests/resource
call %BatchToRun% || goto :error
cd ../..
cd tests/sad
call %BatchToRun% || goto :error
cd ../..
cd tests/layouts
call %BatchToRun% || goto :error
cd ../..
cd tools/isqt580
qmake CONFIG+=%QtConfig% isqt580.pro || goto :error
mingw32-make || goto :error
cd ../..
%CHECKQTVERTOOL%
if errorlevel 1 (
  cd plugins/qt-widget
  qmake CONFIG+=%QtConfig% qt-widget.pro || goto :error
  mingw32-make || goto :error
  cd ../..
  cd examples/example-game-in-qt
  qmake CONFIG+=%QtConfig% example-game-in-qt.pro || goto :error
  mingw32-make || goto :error
  cd ../..
)
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
